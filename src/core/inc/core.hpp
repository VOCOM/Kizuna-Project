#ifndef CORE
#define CORE

#include <exception>

#include <CL/opencl.hpp>

class Core_Bond {
public:
    static const int MAX_BUFFER_COUNT = 5;
    static const int MAX_BUFFER_SIZE = 1024;

public:
    static bool Init();
    static const cl::CommandQueue& Queue() { return _queue; }
    static cl::Buffer& Buffer(int idx) {
        if (idx > MAX_BUFFER_COUNT)throw new std::exception();
        return _buffers[idx];
    }
    static int MAX_THREADS() { return _max_threads; }

    static cl::Kernel& Add() { return _add; }
    static cl::Kernel& Sub() { return _sub; }
    static cl::Kernel& Mul() { return _mul; }
    static cl::Kernel& Transpose() { return _tra; }

private:
    static bool LoadPlatform();
    static bool LoadDevice(uint8_t cl_device_type);
    static bool BuildProgram(cl::Context& context, cl::Device& device, std::string source_path, cl::Program& program);

private:
    static cl::Buffer _buffers[MAX_BUFFER_COUNT];
    static int _max_threads;

    static cl::CommandQueue _queue;
    static cl::Kernel _add;
    static cl::Kernel _sub;
    static cl::Kernel _mul;
    static cl::Kernel _tra;
};

#endif /* CORE */
