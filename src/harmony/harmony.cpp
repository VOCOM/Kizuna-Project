#include <harmony.hpp>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <kizuna/core.hpp>
#include <unsupervised.hpp>
#include <utility/utils.hpp>

std::thread Harmony::mainThread;
std::vector<std::thread> Harmony::workerThreads;

int Harmony::MAX_CPU_THREADS;
int Harmony::MAX_GPU_THREADS;
int Harmony::MAX_BUFFER_COUNT;
int Harmony::MAX_BUFFER_SIZE;

cl::Device Harmony::device;
cl::Context Harmony::context;
std::vector<cl::Buffer> Harmony::buffers;
cl::CommandQueue Harmony::hardwareQueue;
cl::Kernel Harmony::euclid;
cl::Kernel Harmony::centroid;

std::atomic_bool Harmony::dataLock = false;
DataTable Harmony::data;
Results Harmony::results;
std::queue<Results (*)(DataTable&, int)> Harmony::runQueue;

void Harmony::Info() {
	std::cout << "Submodule " << Name() << "\n";
	std::cout << "Status " << Status() << "\n";
	int typeVal = cl::Device::getDefault().getInfo<CL_DEVICE_TYPE>();
	std::string type("CPU");
	switch (typeVal) {
	case CL_DEVICE_TYPE_GPU:
		type = "GPU";
		break;
	case CL_DEVICE_TYPE_ACCELERATOR:
		type = "Accelerator";
		break;
	case CL_DEVICE_TYPE_CUSTOM:
		type = "Custom";
		break;
	}
	std::cout << "Hardware " << type << "\n";
	std::cout << "\n";
}
void Harmony::Start() {
	if (status == Online) return;
	status     = Online;
	mainThread = std::thread(&Harmony::Loop, this);
	std::cout << "Harmony online\n";
}
void Harmony::Stop() {
	if (status == Offline) return;
	status = Offline;
	if (mainThread.joinable()) mainThread.join();
}
void Harmony::Restart() {
	Stop();
	Start();
}
void Harmony::LoadConfiguration() {
	std::string param;
	auto& config = Configuration::Config["harmony"];

	if (config.find("max_cpu_threads") == config.end())
		config["max_cpu_threads"] = "auto";
	param           = config["max_cpu_threads"];
	MAX_CPU_THREADS = param == "auto" ? std::thread::hardware_concurrency() : std::stoi(param);

	if (config.find("buffer_size") == config.end())
		config["buffer_size"] = "0xFFFFF";
	param           = config["buffer_size"];
	int base        = param.find('x') == std::string::npos ? 10 : 16;
	MAX_BUFFER_SIZE = std::stoi(param, nullptr, base);

	if (config.find("buffer_count") == config.end())
		config["buffer_count"] = "5";
	MAX_BUFFER_COUNT = std::stoi(config["buffer_count"]);

	// Initialize worker pool
	workerThreads.resize(MAX_CPU_THREADS);

	// Initialize buffers
	while (buffers.size()) buffers.pop_back();
	for (int i = 0; i < MAX_BUFFER_COUNT; i++)
		buffers.push_back(cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * MAX_BUFFER_SIZE));
}

void Harmony::ShellHeader() {
	if (dataLock) std::cout << "Harmony executing...\n";
	while (dataLock) std::this_thread::yield();

	if (data.Cols() > 0) {
		std::cout << "Data:\n";
		data.ShortInfo(3);
	}

	if (!results.Empty()) {
		std::cout << "Results:\n";
		auto headers = data.Header();

		for (int c = 0; c < headers.size(); c++) {
			std::cout << '[' << headers[c] << "]\n";

			auto values   = results.clusters[c];
			int entries   = values.rowwise().count().count();
			int dimension = values.cols();
			for (int i = 0; i < entries; i++) {
				for (int j = 0; j < dimension; j++)
					std::cout << values(i, j) << " ";
				std::cout << "\n";
			}
			std::cout << "\n";
		}
	}

	std::cout << name << ": ";
}
void Harmony::Shell(std::string command, std::queue<std::string> params) {
	if (command == "load") {
		std::ifstream fs(params.front());
		std::stringstream ss;
		ss << fs.rdbuf();

		std::string line;
		ss >> line;
		auto headers = Split(line, ',');
		for (auto& header : headers) data.AddFeature(header);

		while (!ss.eof()) {
			ss >> line;
			std::vector<std::string> stringValues = Split(line, ',');
			std::vector<double> values;
			for (auto& v : stringValues) values.push_back(std::stod(v));
			data.AddElements(values);
		}
	}
	if (command == "run") {
		auto& model = params.front();
		if (model == "kmeans") runQueue.push(KMeans);
	}
}

Harmony::Harmony() : Submodule("Harmony") {
	// Load OpenCL Supported Hardware
	if (LoadPlatform() == false) {
		std::cout << "Error registering computing platform.\n";
		return;
	}

	// Load GPU Hardware
	if (LoadDevice(CL_DEVICE_TYPE_GPU) == false) {
		std::cout << "Error registering computing device.\n";
		return;
	}

	// Generate Context
	context = cl::Context(device);
	if (cl::Context::setDefault(context) != context)
		std::cout << "Error Setting Default Context.\n";

	// Initialize Queue
	hardwareQueue = cl::CommandQueue(context, device);

	// Compile Core Program
	cl::Program core_program;
	if (BuildProgram(context, device, ".\\harmony\\math.cl", core_program) == false) {
		std::cout << "Error Building Program\n";
		return;
	}

	// Load Kernel Functions
	int ret;
	euclid = cl::Kernel(core_program, "euclideanDistance", &ret);
	if (ret != CL_SUCCESS)
		std::cout << "Failed to load Euclidean Distance kernel. Code " << ret << "\n";
	centroid = cl::Kernel(core_program, "centroid", &ret);
	if (ret != CL_SUCCESS)
		std::cout << "Failed to load Centroid kernel. Code " << ret << "\n";
}
Harmony::~Harmony() {
	Stop();
}

void Harmony::Loop() {
	try {
		while (status != Offline) {
			if (runQueue.empty()) {
				dataLock = false;
				std::this_thread::yield();
				continue;
			}

			auto& model = runQueue.front();
			runQueue.pop();

			dataLock = true;
			results  = model(data, 4);
			std::cout << "Completed " << results.Type() << " model\n";
		}
		std::cout << "Harmony terminating...\n";
	} catch (Error& e) {
		status = Faulted;
		Kizuna::ErrorQueue.push(e);
	}
}
cl::Buffer& Harmony::Buffer(int idx) {
	if (idx > MAX_BUFFER_COUNT) throw new std::exception();
	hardwareQueue.enqueueFillBuffer(buffers[idx], 0, 0, sizeof(double) * MAX_BUFFER_SIZE);
	return buffers[idx];
}

bool Harmony::LoadPlatform() {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	cl::Platform platform;

	for (auto& p : platforms) {
		std::string platver = p.getInfo<CL_PLATFORM_VERSION>();
		if (platver.find("OpenCL 2.") != std::string::npos ||
		    platver.find("OpenCL 3.") != std::string::npos) {
			// Note: an OpenCL 3.x platform may not support all required features!
			platform = p;
		}
	}

	if (platform() == 0) return false;
	return cl::Platform::setDefault(platform) == platform;
}
bool Harmony::LoadDevice(uint8_t cl_device_type) {
	std::vector<cl::Device> devices;
	cl::Platform::getDefault().getDevices(CL_DEVICE_TYPE_ALL, &devices);
	if (devices.size() == 0) return false;

	for (auto& d : devices) {
		if (d.getInfo<CL_DEVICE_TYPE>() != cl_device_type) continue;
		if (cl::Device::setDefault(d) != d)
			std::cout << "Error Setting default device.\n";
		device = d;
		return true;
	}
	return false;
}
bool Harmony::BuildProgram(cl::Context& context, cl::Device& device, std::string source_path, cl::Program& program) {
	std::ifstream fs(source_path);
	std::stringstream kernel_code;
	kernel_code << fs.rdbuf();
	cl::Program::Sources source{kernel_code.str()};
	program = cl::Program(context, source);
	return program.build(device) == CL_SUCCESS;
}
