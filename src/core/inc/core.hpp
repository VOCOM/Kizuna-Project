#ifndef CORE
#define CORE

#include <exception>

#include <CL/opencl.hpp>

class Core_Bond {
public:
    static const int MAX_BUFFER_COUNT = 5;
    static const int MAX_BUFFER_SIZE = 0xFFFFFF;

public:
    static bool Init();
    static const cl::CommandQueue& Queue() { return _queue; }
    static cl::Buffer& Buffer(int idx) {
        if (idx > MAX_BUFFER_COUNT)throw new std::exception();
        _queue.enqueueFillBuffer(_buffers[idx], 0, 0, sizeof(int) * MAX_BUFFER_SIZE);
        return _buffers[idx];
    }
    static int MAX_THREADS() { return _max_threads; }

    static cl::Kernel& Distance() { return _euclid; }
    static cl::Kernel& Centroid() { return _centroid; }

private:
    static bool LoadPlatform();
    static bool LoadDevice(uint8_t cl_device_type);
    static bool BuildProgram(cl::Context& context, cl::Device& device, std::string source_path, cl::Program& program);

private:
    static cl::Buffer _buffers[MAX_BUFFER_COUNT];
    static int _max_threads;

    static cl::CommandQueue _queue;

    static cl::Kernel _euclid;
    static cl::Kernel _centroid;
};

#endif /* CORE */
