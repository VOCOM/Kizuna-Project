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

using namespace std;

thread Harmony::mainThread;
vector<thread> Harmony::workerThreads;

int Harmony::MAX_CPU_THREADS;
int Harmony::MAX_GPU_THREADS;
int Harmony::MAX_BUFFER_COUNT;
int Harmony::MAX_BUFFER_SIZE;

cl::Device Harmony::device;
cl::Context Harmony::context;
vector<cl::Buffer> Harmony::buffers;
cl::CommandQueue Harmony::hardwareQueue;
cl::Kernel Harmony::euclid;
cl::Kernel Harmony::centroid;

DataTable Harmony::holdingData;
shared_ptr<Model> Harmony::currentModel = nullptr;
queue<shared_ptr<Model>> Harmony::runQueue;
vector<shared_ptr<Model>> Harmony::completedQueue;

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
	cout << "Harmony online\n";
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
	param           = config["max_cpu_threads"];
	MAX_CPU_THREADS = param == "auto" ? thread::hardware_concurrency() : stoi(param);

	if (config.find("buffer_size") == config.end())
		config["buffer_size"] = "0xFFFFF";
	param           = config["buffer_size"];
	int base        = param.find('x') == string::npos ? 10 : 16;
	MAX_BUFFER_SIZE = stoi(param, nullptr, base);

	if (config.find("buffer_count") == config.end())
		config["buffer_count"] = "5";
	MAX_BUFFER_COUNT = stoi(config["buffer_count"]);

	// Initialize worker pool
	workerThreads.resize(MAX_CPU_THREADS);

	// Initialize buffers
	while (buffers.size()) buffers.pop_back();
	for (int i = 0; i < MAX_BUFFER_COUNT; i++)
		buffers.push_back(cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * MAX_BUFFER_SIZE));
}

void Harmony::ShellHeader() {
	cout << "Completed Queue:\n";
	if (completedQueue.size()) {
		cout << completedQueue.size() << " models completed.\n";
	} else {
		cout << "Empty\n";
	}

	cout << "\nSelected model:\n";
	if (currentModel != nullptr) {
		cout << currentModel->Name() << "\n";
	} else {
		cout << "None\n";
	}

	cout << "\nData:\n";
	if (holdingData.Count()) {
		holdingData.Info(3);
	} else {
		cout << "Empty\n";
	}

	// cout << "Results:\n";
	// if (!results.Empty()) {
	// 	auto headers = data.GetHeader();
	// 	for (int c = 0; c < headers.size(); c++) {
	// 		cout << '[' << headers[c] << "]\n";
	// 		auto values   = results.clusters[c];
	// 		int entries   = values.rowwise().count().count();
	// 		int dimension = values.cols();
	// 		for (int i = 0; i < entries; i++) {
	// 			for (int j = 0; j < dimension; j++)
	// 				cout << values(i, j) << " ";
	// 			cout << "\n";
	// 		}
	// 		cout << "\n";
	// 	}
	// }

	cout << "\n";
	cout << name << ": ";
}
void Harmony::Shell(string command, queue<string> params) {
	if (command == "select") {
		if (params.size() < 2) return;
		auto type = params.front();
		params.pop();
		if (type == "kmeans") {
			currentModel                                   = make_shared<KMeansc>();
			dynamic_pointer_cast<KMeansc>(currentModel)->k = stoi(params.front());
		}
	}
	if (command == "deselect")
		currentModel = nullptr;
	if (command == "load")
		holdingData.LoadCSV(params.front());
	if (command == "unload")
		holdingData.DropData();
	if (command == "run" && currentModel != nullptr) {
		currentModel->Load(holdingData);
		holdingData.DropData();
		runQueue.push(currentModel);
		currentModel = nullptr;
	}
}

Harmony::Harmony() : Submodule("Harmony") {
	// Load OpenCL Supported Hardware
	if (LoadPlatform() == false) {
		cout << "Error registering computing platform.\n";
		return;
	}

	// Load GPU Hardware
	if (LoadDevice(CL_DEVICE_TYPE_GPU) == false) {
		cout << "Error registering computing device.\n";
		return;
	}

	// Generate Context
	context = cl::Context(device);
	if (cl::Context::setDefault(context) != context)
		cout << "Error Setting Default Context.\n";

	// Initialize Queue
	hardwareQueue = cl::CommandQueue(context, device);

	// Compile Core Program
	cl::Program core_program;
	if (BuildProgram(context, device, ".\\harmony\\math.cl", core_program) == false) {
		cout << "Error Building Program\n";
		return;
	}

	// Load Kernel Functions
	int ret;
	euclid = cl::Kernel(core_program, "euclideanDistance", &ret);
	if (ret != CL_SUCCESS)
		cout << "Failed to load Euclidean Distance kernel. Code " << ret << "\n";
	centroid = cl::Kernel(core_program, "centroid", &ret);
	if (ret != CL_SUCCESS)
		cout << "Failed to load Centroid kernel. Code " << ret << "\n";
}
Harmony::~Harmony() {
	Stop();
}

void Harmony::Loop() {
	try {
		while (status != Offline) {
			if (runQueue.empty()) {
				this_thread::yield();
				continue;
			}

			auto model = runQueue.front();
			runQueue.pop();

			model->Execute();
			completedQueue.push_back(model);
		}
		cout << "Harmony terminating...\n";
	} catch (Error& e) {
		status = Faulted;
		Kizuna::ErrorQueue.push(e);
	}
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
		if (cl::Device::setDefault(d) != d)
			cout << "Error Setting default device.\n";
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
