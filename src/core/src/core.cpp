#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <core.hpp>

cl::Buffer Core_Bond::_buffers[Core_Bond::MAX_BUFFER_COUNT];
int Core_Bond::_max_threads = 64;

cl::CommandQueue Core_Bond::_queue;
cl::Kernel Core_Bond::_add;
cl::Kernel Core_Bond::_sub;
cl::Kernel Core_Bond::_mul;
cl::Kernel Core_Bond::_tra;

bool Core_Bond::Init() {
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
    _max_threads = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

    // Compile Core Program
    cl::Context context(device);
    cl::Program core_program;
    if (BuildProgram(context, device, ".\\core\\math.cl", core_program) == false) {
        std::cout << "Error Building Program\n";
        return false;
    }

    // Initialize buffers
    for (int i = 0;i < 5;i++)
        _buffers[i] = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * MAX_BUFFER_SIZE);

    // Load Kernel Functions
    _add = cl::Kernel(core_program, "add");
    _sub = cl::Kernel(core_program, "sub");
    _mul = cl::Kernel(core_program, "mul");
    _tra = cl::Kernel(core_program, "transpose");

    // Initialize Queue
    _queue = cl::CommandQueue(context, device);

    return true;
}
bool Core_Bond::LoadPlatform() {
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
bool Core_Bond::LoadDevice(uint8_t cl_device_type) {
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
bool Core_Bond::BuildProgram(cl::Context& context, cl::Device& device, std::string source_path, cl::Program& program) {
    std::ifstream fs(source_path);
    std::stringstream kernel_code;
    kernel_code << fs.rdbuf();
    cl::Program::Sources source{ kernel_code.str() };
    program = cl::Program(context, source);
    return program.build(device) == CL_SUCCESS;
}
