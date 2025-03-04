#include "harmony.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <kizuna.hpp>

bool Harmony::running;
std::thread Harmony::mainThread;

int Harmony::MAX_CPU_THREADS;
int Harmony::MAX_GPU_THREADS;
int Harmony::MAX_BUFFER_COUNT;
int Harmony::MAX_BUFFER_SIZE;

std::vector<cl::Buffer> Harmony::buffers;
cl::CommandQueue Harmony::queue;
cl::Kernel Harmony::euclid;
cl::Kernel Harmony::centroid;

void Harmony::Start() {
	running    = true;
	mainThread = std::thread(&Harmony::Loop);
}

Harmony::Harmony() {
	// Load Configuration
	LoadConfiguration();

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
	cl::Device device = cl::Device::getDefault();
	cl::Context context(device);

	// Initialize Queue
	queue = cl::CommandQueue(context, device);

	// Initialize buffers
	for (int i = 0; i < MAX_BUFFER_COUNT; i++)
		buffers.push_back(cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * MAX_BUFFER_SIZE));

	// Compile Core Program
	cl::Program core_program;
	if (BuildProgram(context, device, ".\\harmony\\math.cl", core_program) == false) {
		std::cout << "Error Building Program\n";
		return;
	}

	// Load Kernel Functions
	euclid   = cl::Kernel(core_program, "euclideanDistance");
	centroid = cl::Kernel(core_program, "centroid");
}
Harmony::~Harmony() {
	running = false;
	if (mainThread.joinable()) mainThread.join();
}

void Harmony::Loop() {
	while (running) {}
	std::cout << "Harmony terminating...\n";
}
cl::Buffer& Harmony::Buffer(int idx) {
	if (idx > MAX_BUFFER_COUNT) throw new std::exception();
	queue.enqueueFillBuffer(buffers[idx], 0, 0, sizeof(int) * MAX_BUFFER_SIZE);
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
		cl::Device::setDefault(d);
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

void Harmony::LoadConfiguration() {
	std::string param;
	auto& config = Configuration::Config["harmony"];

	param           = config["max_cpu_threads"];
	MAX_CPU_THREADS = param == "auto" ? std::thread::hardware_concurrency() : std::stoi(param);

	param           = config["buffer_size"];
	MAX_BUFFER_SIZE = std::stoi(param, nullptr, param.find('x') == std::string::npos ? 10 : 16);

	param            = config["buffer_count"];
	MAX_BUFFER_COUNT = std::stoi(param);
}
