#include <iostream>
#include <vector>

#include <CL/opencl.hpp>

bool LoadPlatform() {
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
bool LoadDevice(cl::Platform& platform, uint8_t cl_device_type) {
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if (devices.size() == 0) return false;

    for (auto& d : devices) {
        if (d.getInfo<CL_DEVICE_TYPE>() != cl_device_type) continue;
        cl::Device::setDefault(d);
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    if (LoadPlatform() == false) {
        std::cout << "Error registering computing platform.\n";
        return 0;
    }

    cl::Platform platform = cl::Platform::getDefault();
    if (LoadDevice(platform, CL_DEVICE_TYPE_GPU) == false) {
        std::cout << "Error registering computing device.\n";
        return 0;
    }
}