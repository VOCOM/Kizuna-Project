#ifndef OPENCL_KERNELS
#define OPENCL_KERNELS

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

namespace Kernels {
bool Initialize(uint8_t deviceType);
bool LoadKernels(const char* source_path);
void LoadBuffers(int bufferCount, int bufferSize);

cl::Kernel& ReLu();
cl::Kernel& Distance();
cl::Kernel& Centroid();
cl::Kernel& Perceptron();

cl::CommandQueue& Queue();
cl::Buffer& Buffer(int idx);
} // namespace Kernels

#endif /* OPENCL_KERNELS */
