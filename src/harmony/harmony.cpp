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

	if (config.find("buffer_count") == config.end()) config["buffer_count"] = "5";
	if (config.find("buffer_size") == config.end()) config["buffer_size"] = "0xFFFFF";
	int base = config["buffer_size"].find('x') == string::npos ? 10 : 16;
	Kernels::LoadBuffers(stoi(config["buffer_count"]), stoi(param, nullptr, base));

	// Initialize worker pool
	workerThreads.resize(maxCpuThreads);
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

// Neural Net Operations
void Harmony::NNOperations() {}

// Constructors
Harmony::Harmony() : Module("Harmony") {
	if (Kernels::Initialize(CL_DEVICE_TYPE_GPU))
		Raise("Error Initializing OpenCL.");

	// Load Kernel Functions
	if (Kernels::LoadKernels(".\\harmony\\math.cl"))
		Raise("Error Initializing Kernel Functions.");
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
