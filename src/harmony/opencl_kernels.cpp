#include <opencl_kernels.hpp>

#include <fstream>
#include <sstream>

#include <utility/utils.hpp>

namespace Kernels {
int MAX_BUFFER_COUNT;
int MAX_BUFFER_SIZE;

cl::Kernel relu;
cl::Kernel euclid;
cl::Kernel centroid;
cl::Kernel perceptron;

cl::Device device;
cl::Context context;
cl::CommandQueue hardwareQueue;
std::vector<cl::Buffer> buffers;

cl::Kernel& ReLu() { return relu; }
cl::Kernel& Distance() { return euclid; }
cl::Kernel& Centroid() { return centroid; }
cl::Kernel& Perceptron() { return perceptron; }

cl::CommandQueue& Queue() { return hardwareQueue; }

cl::Buffer& Buffer(int idx) {
	if (idx > MAX_BUFFER_COUNT) throw std::exception();
	hardwareQueue.enqueueFillBuffer(buffers[idx], 0, 0, sizeof(double) * MAX_BUFFER_SIZE);
	return buffers[idx];
}

bool Initialize(uint8_t cl_device_type) {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	cl::Platform platform;

	// Note: an OpenCL 3.x platform may not support all required features!
	for (auto& p : platforms) {
		std::string version = p.getInfo<CL_PLATFORM_VERSION>();
		if (Contains(version, "OpenCL 2.") || Contains(version, "OpenCL 3.")) platform = p;
	}
	if (platform() == 0) return false;
	cl::Platform::setDefault(platform);

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	if (devices.size() == 0) return false;

	for (auto& d : devices)
		if (d.getInfo<CL_DEVICE_TYPE>() == cl_device_type) device = d;
	if (device() == 0) false;

	context       = cl::Context(device);
	hardwareQueue = cl::CommandQueue(context, device);
	return true;
}
bool LoadKernels(const char* source_path) {
	std::ifstream fs(source_path);
	std::stringstream kernel_code;
	kernel_code << fs.rdbuf();
	cl::Program program = (context, cl::Program::Sources{kernel_code.str()});
	if (program.build(device) != CL_SUCCESS) return false;

	int ret       = 0;
	Kernels::relu = cl::Kernel(program, "relu", &ret);
	if (ret) return ret;
	Kernels::perceptron = cl::Kernel(program, "perceptron", &ret);
	if (ret) return ret;
	Kernels::centroid = cl::Kernel(program, "centroid", &ret);
	if (ret) return ret;
	Kernels::euclid = cl::Kernel(program, "euclideanDistance", &ret);
	return ret == 0;
}
void LoadBuffers(int bufferCount, int bufferSize) {
	MAX_BUFFER_COUNT = bufferCount;
	MAX_BUFFER_SIZE  = bufferSize;

	while (buffers.size()) buffers.pop_back();
	for (int i = 0; i < MAX_BUFFER_COUNT; i++)
		buffers.push_back(cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * MAX_BUFFER_SIZE));
}
} // namespace Kernels
