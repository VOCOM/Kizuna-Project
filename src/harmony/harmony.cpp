#include <harmony.hpp>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <kizuna/configuration.hpp>
#include <neuralnet/neuralnet.hpp>
#include <unsupervised.hpp>
#include <utility/utils.hpp>

using namespace std;

int Harmony::MAX_BUFFER_COUNT;
int Harmony::MAX_BUFFER_SIZE;

cl::Device Harmony::device;
cl::Context Harmony::context;
vector<cl::Buffer> Harmony::buffers;
cl::CommandQueue Harmony::hardwareQueue;
cl::Kernel Harmony::euclid;
cl::Kernel Harmony::centroid;
cl::Kernel Harmony::perceptron;
cl::Kernel Harmony::relu;

// Submodule Interface
void Harmony::Info() {
	cout << "Submodule " << Name() << "\n";
	cout << "Status " << Status() << "\n";
	int typeVal = cl::Device::getDefault().getInfo<CL_DEVICE_TYPE>();
	string type("CPU");
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
	cout << "Hardware " << type << "\n";
	cout << "\n";
}
void Harmony::Start() {
	if (status == Online) return;
	status     = Online;
	mainThread = thread(&Harmony::Loop, this);
	cout << "Harmony Online\n";
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
	string param;
	auto& config = Configuration::Config["harmony"];

	if (config.find("max_cpu_threads") == config.end())
		config["max_cpu_threads"] = "auto";
	param         = config["max_cpu_threads"];
	maxCpuThreads = param == "auto" ? thread::hardware_concurrency() : stoi(param);

	if (config.find("buffer_size") == config.end())
		config["buffer_size"] = "0xFFFFF";
	param           = config["buffer_size"];
	int base        = param.find('x') == string::npos ? 10 : 16;
	MAX_BUFFER_SIZE = stoi(param, nullptr, base);

	if (config.find("buffer_count") == config.end())
		config["buffer_count"] = "5";
	MAX_BUFFER_COUNT = stoi(config["buffer_count"]);

	// Initialize worker pool
	workerThreads.resize(maxCpuThreads);

	// Initialize buffers
	while (buffers.size()) buffers.pop_back();
	for (int i = 0; i < MAX_BUFFER_COUNT; i++)
		buffers.push_back(cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * MAX_BUFFER_SIZE));
}

// Kernel Interface
void Harmony::Access() {
	string buffer, command;
	queue<string> params;

	while (true) {
		// Header
		// Clear();
		// if (lock) std::cout << " Harmony Executing...\n";
		while (lock) std::this_thread::yield();

		cout << "Completed Queue: " << completedQueue.size() << "\n";
		cout << "Selected model: " << (currentModel != nullptr ? currentModel->Name() : "None") << "\n";
		cout << "Data:\n";
		holdingData.Info(3);

		// Input
		std::cout << "Harmony: ";
		getline(cin, buffer);
		std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);
		params  = Enqueue(buffer);
		command = params.front();
		params.pop();

		// Kernel Commands
		if (command == "exit") return;

		// Body
		if (command == "select") {
			auto type = params.front();
			params.pop();
			if (type == "kmeans") {
				if (params.size() < 2) continue;
				auto model   = make_shared<KMeans>();
				model->k     = stoi(params.front());
				currentModel = model;
			}
			if (type == "net") {
				auto model = make_shared<NeuralNet>();
				model->AddLayer(2);
				model->AddLayer(1);
				currentModel = model;
			}
		}
		if (command == "deselect") currentModel = nullptr;
		if (command == "load") holdingData.LoadCSV(params.front());
		if (command == "unload") holdingData.DropData();
		if (command == "run" && currentModel != nullptr) {
			currentModel->Load(holdingData);
			holdingData.DropData();
			runQueue.push(currentModel);
			currentModel = nullptr;
		}
	}
}

// Constructors
Harmony::Harmony() : Emitter("Harmony") {
	// modules.push_back(std::make_shared<Harmony>(*this));

	// Load OpenCL Supported Hardware
	if (LoadPlatform() == false) {
		Raise("Error registering computing platform.");
		return;
	}

	// Load GPU Hardware
	if (LoadDevice(CL_DEVICE_TYPE_GPU) == false) {
		Raise("Error registering computing device.");
		return;
	}

	// Generate Context
	context = cl::Context(device);

	// Initialize Queue
	hardwareQueue = cl::CommandQueue(context, device);

	// Compile Core Program
	cl::Program core_program;
	if (BuildProgram(context, device, ".\\harmony\\math.cl", core_program) == false) {
		Raise("Error Building Program.");
		return;
	}

	// Load Kernel Functions
	int ret;
	char buffer[100];
	euclid = cl::Kernel(core_program, "euclideanDistance", &ret);
	if (ret != CL_SUCCESS) {
		std::snprintf(buffer, 100, "Failed to load Euclidean Distance kernel. Code %d", ret);
		Raise(buffer);
	}
	centroid = cl::Kernel(core_program, "centroid", &ret);
	if (ret != CL_SUCCESS) {
		std::snprintf(buffer, 100, "Failed to load Centroid kernel. Code %d", ret);
		Raise(buffer);
	}
	perceptron = cl::Kernel(core_program, "perceptron", &ret);
	if (ret != CL_SUCCESS) {
		std::snprintf(buffer, 100, "Failed to load Perceptron kernel. Code %d", ret);
		Raise(buffer);
	}
	relu = cl::Kernel(core_program, "relu", &ret);
	if (ret != CL_SUCCESS) {
		std::snprintf(buffer, 100, "Failed to load ReLu kernel. Code %d", ret);
		Raise(buffer);
	}
}
Harmony::~Harmony() {
	Stop();
}

// Private Methods
void Harmony::Loop() {
	while (status != Offline) {
		if (runQueue.empty()) {
			lock = false;
			this_thread::yield();
			continue;
		}

		auto model = runQueue.front();
		runQueue.pop();

		lock = true;
		model->Train(maxCpuThreads);
		completedQueue.push_back(model);
	}
	cout << "Harmony terminating...\n";
}

cl::Buffer& Harmony::Buffer(int idx) {
	if (idx > MAX_BUFFER_COUNT) throw new exception();
	hardwareQueue.enqueueFillBuffer(buffers[idx], 0, 0, sizeof(double) * MAX_BUFFER_SIZE);
	return buffers[idx];
}

bool Harmony::LoadPlatform() {
	vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	cl::Platform platform;

	for (auto& p : platforms) {
		string platver = p.getInfo<CL_PLATFORM_VERSION>();
		if (platver.find("OpenCL 2.") != string::npos ||
		    platver.find("OpenCL 3.") != string::npos) {
			// Note: an OpenCL 3.x platform may not support all required features!
			platform = p;
		}
	}

	if (platform() == 0) return false;
	return cl::Platform::setDefault(platform) == platform;
}
bool Harmony::LoadDevice(uint8_t cl_device_type) {
	vector<cl::Device> devices;
	cl::Platform::getDefault().getDevices(CL_DEVICE_TYPE_ALL, &devices);
	if (devices.size() == 0) return false;

	for (auto& d : devices) {
		if (d.getInfo<CL_DEVICE_TYPE>() != cl_device_type) continue;
		device = d;
		return true;
	}
	return false;
}
bool Harmony::BuildProgram(cl::Context& context, cl::Device& device, string source_path, cl::Program& program) {
	ifstream fs(source_path);
	stringstream kernel_code;
	kernel_code << fs.rdbuf();
	cl::Program::Sources source{kernel_code.str()};
	program = cl::Program(context, source);
	return program.build(device) == CL_SUCCESS;
}
