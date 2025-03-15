#include <neuralnet/layers.hpp>

#include <iostream>

#include <harmony.hpp>

int j = 0;

cl::Buffer ReLuLayer::Compute(cl::Buffer inputs, std::vector<double> inputWeights, int count) {
	// Buffer 0 is inputs
	int depth    = Depth();
	auto queue   = Harmony::Queue();
	auto weights = Harmony::Buffer(1);
	auto biases  = Harmony::Buffer(2);
	auto outputs = Harmony::Buffer(3);

	// Load Buffers
	int ret = queue.enqueueWriteBuffer(weights, CL_TRUE, 0, sizeof(double) * count, inputWeights.data());
	if (ret != CL_SUCCESS) std::cout << "Error Filling Weights buffer. Code " << ret << "\n";
	ret = queue.enqueueWriteBuffer(biases, CL_TRUE, 0, sizeof(double) * bias.size(), bias.data());
	if (ret != CL_SUCCESS) std::cout << "Error Filling Biases buffer. Code " << ret << "\n";

	// Bind Kernel Parameters (Perceptrons)
	auto kernel = Harmony::Perceptron();
	kernel.setArg(0, inputs);
	kernel.setArg(1, weights);
	kernel.setArg(2, count);
	kernel.setArg(3, biases);
	kernel.setArg(4, outputs);

	// Execute Kernel
	ret = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(depth));
	if (ret != CL_SUCCESS) std::cout << "Error executing " << Name() << ". Code " << ret << "\n";
	queue.finish();

	// Bind Kernel Parameters (Activation)
	kernel = Harmony::ReLu();
	inputs = Harmony::Buffer(0);
	kernel.setArg(0, outputs);
	kernel.setArg(1, inputs);

	// Execute Kernel
	ret = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(depth));
	if (ret != CL_SUCCESS) std::cout << "Error executing " << Name() << ". Code " << ret << "\n";
	queue.finish();

	return inputs;
}