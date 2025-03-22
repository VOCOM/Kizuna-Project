#include <harmony.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <kizuna/configuration.hpp>
#include <neuralnet/layers.hpp>
#include <neuralnet/neuralnet.hpp>
#include <unsupervised.hpp>
#include <utility/utils.hpp>

// Submodule Interface
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
	std::cout << "Harmony Online\n";
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
	param         = config["max_cpu_threads"];
	maxCpuThreads = param == "auto" ? std::thread::hardware_concurrency() : stoi(param);

	if (config.find("buffer_count") == config.end()) config["buffer_count"] = "5";
	if (config.find("buffer_size") == config.end()) config["buffer_size"] = "0xFFFFF";
	int base = config["buffer_size"].find('x') == std::string::npos ? 10 : 16;
	Kernels::LoadBuffers(stoi(config["buffer_count"]), stoi(param, nullptr, base));

	// Initialize worker pool
	workerThreads.resize(maxCpuThreads);
}

// Kernel Interface
void Harmony::Access() {
	std::string command;

	while (true) {
		while (lock) std::this_thread::yield();

		std::cout << "Completed Queue: " << completedQueue.size() << "\n";

		std::cout << "Model: ";
		if (currentModel) currentModel->Info();
		else std::cout << "None\n";

		std::cout << "Data:\n";
		holdingData.Info(3);

		// Input
		std::cout << "Harmony: ";
		std::cin >> buffer;
		buffer >> command;

		// Kernel Commands
		if (command == "exit") {
			currentModel = nullptr;
			return;
		}
		if (command == "clear") Clear();

		// Model
		if (command == "select") {
			std::string type = buffer.pop();
			if (type == "kmeans") {
				if (buffer.size() < 2) continue;

				auto model   = std::make_shared<KMeans>();
				model->k     = ToInt(buffer.pop());
				currentModel = model;
			}
			if (type == "net") NNOperations();
		}
		if (command == "deselect") currentModel = nullptr;

		// Data
		if (command == "load") holdingData.LoadCSV(buffer.pop());
		if (command == "unload") holdingData.DropData();

		// Execute
		if (command == "run" && currentModel) {
			currentModel->Load(holdingData);
			holdingData.DropData();
			runQueue.push(currentModel);
			currentModel = nullptr;
		}
	}
}

// Neural Net Operations
void Harmony::NNOperations() {
	auto model = std::make_shared<NeuralNet>();

	std::string command;
	while (true) {
		model->Info();
		std::cin >> buffer;
		std::string command = buffer.pop();

		if (command == "save") break;
		if (command == "cancel") return;

		if (command == "add") {
			if (buffer.size() < 2) continue;
			std::string type = buffer.pop();
			int depth        = ToInt(buffer.pop());

			std::unique_ptr<Layer> layer;

			if (Contains(ToLower(typeid(ReLuLayer).name()), type))
				layer = std::make_unique<ReLuLayer>(depth);

			if (layer)
				model->AddLayer(std::move(layer));
		}
		if (command == "del") {}
	}

	currentModel = model;
}

// Constructors
Harmony::Harmony() : Module("Harmony"), maxCpuThreads(), maxGpuThreads(), status() {
	if (Kernels::Initialize(CL_DEVICE_TYPE_GPU)) Raise("Error Initializing OpenCL.");

	// Load Kernel Functions
	if (Kernels::LoadKernels(".\\opencl\\math.cl")) Raise("Error Initializing Kernel Functions.");
}
Harmony::~Harmony() {
	Stop();
}

// Private Methods
void Harmony::Loop() {
	while (status != Offline) {
		if (runQueue.empty()) {
			lock = false;
			std::this_thread::yield();
			continue;
		}

		auto model = runQueue.front();
		runQueue.pop();

		lock = true;
		model->Train(maxCpuThreads);
		completedQueue.push_back(model);
	}
	std::cout << "Harmony terminating...\n";
}
