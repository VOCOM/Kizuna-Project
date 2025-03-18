#include <neuralnet/hidden_layers.hpp>

#include <iostream>

#include <kernels.hpp>

cl::Buffer ReLuLayer::Compute(cl::Buffer inputs, std::vector<double> inputWeights, int count) {
	// Bind Kernel Parameters (Activation)
	auto kernel  = Kernels::ReLu();
	auto outputs = Kernels::Buffer(1);
	kernel.setArg(0, inputs);

	// Execute Kernel
	auto queue = Kernels::Queue();
	int ret    = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(Depth()));
	if (ret != CL_SUCCESS) std::cout << "Error executing " << Name() << ". Code " << ret << "\n";
	queue.finish();

	return inputs;
}
