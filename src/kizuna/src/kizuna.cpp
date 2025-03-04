#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include <kizuna.hpp>

std::map<std::string, std::map<std::string, std::string>> Configuration::Config;

cl::Buffer Kizuna::_buffers[Kizuna::MAX_BUFFER_COUNT];
int Kizuna::_max_threads = 64;

cl::CommandQueue Kizuna::_queue;

cl::Kernel Kizuna::_euclid;
cl::Kernel Kizuna::_centroid;

void Configuration::ListConfig(std::string filter) {
	std::cout << "Loaded Configuration:\n\n";
	for (auto& modules : Config) {
		if ((filter != "") && (filter != modules.first)) continue;

		std::cout << "Submodule: " << modules.first << "\n";
		for (auto& param : modules.second) {
			std::cout << std::setw(11) << std::left << param.first;
			std::cout << param.second << "\n";
		}
		std::cout << "\n";
	}
}
void Configuration::LoadConfig() {
	std::string submodule, paramLine;
	std::ifstream config("config.ini");
	while (config.eof() == false) {
		config >> paramLine;
		std::transform(paramLine.begin(), paramLine.end(), paramLine.begin(), std::tolower);

		// Comment Line
		if (paramLine.front() == ';') continue;

		// Module Header
		if (paramLine.front() == '[') {
			submodule = paramLine.substr(1, paramLine.size() - 2);
			continue;
		}

		// Extract Parameter
		std::string key, value;
		int delimiter = paramLine.find('=');
		key           = paramLine.substr(0, delimiter);
		value         = paramLine.substr(delimiter + 1, paramLine.size());

		// Store Parameter
		Configuration::Config[submodule][key] = value;
	}
}

bool Kizuna::Init() {
	// Load OpenCL Supported Hardware
	if (LoadPlatform() == false) {
		std::cout << "Error registering computing platform.\n";
		return false;
	}

	// Load GPU Hardware
	if (LoadDevice(CL_DEVICE_TYPE_GPU) == false) {
		std::cout << "Error registering computing device.\n";
		return false;
	}

	// Set Thread Limit
	cl::Device device = cl::Device::getDefault();
	_max_threads      = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

	// Compile Core Program
	cl::Context context(device);
	cl::Program core_program;
	if (BuildProgram(context, device, ".\\kizuna\\math.cl", core_program) == false) {
		std::cout << "Error Building Program\n";
		return false;
	}

	// Initialize buffers
	for (int i = 0; i < 5; i++)
		_buffers[i] = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * MAX_BUFFER_SIZE);

	// Load Kernel Functions
	_euclid   = cl::Kernel(core_program, "euclideanDistance");
	_centroid = cl::Kernel(core_program, "centroid");

	// Initialize Queue
	_queue = cl::CommandQueue(context, device);

	// Load Config
	Configuration::LoadConfig();

	return true;
}
bool Kizuna::LoadPlatform() {
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
bool Kizuna::LoadDevice(uint8_t cl_device_type) {
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
bool Kizuna::BuildProgram(cl::Context& context, cl::Device& device, std::string source_path, cl::Program& program) {
	std::ifstream fs(source_path);
	std::stringstream kernel_code;
	kernel_code << fs.rdbuf();
	cl::Program::Sources source{kernel_code.str()};
	program = cl::Program(context, source);
	return program.build(device) == CL_SUCCESS;
}
