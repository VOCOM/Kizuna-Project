#include <neuralnet/hidden_layers.hpp>

#include <iostream>

#include <kernels.hpp>

int PerceptronLayer::Depth() const { return bias.size(); }
std::vector<double> PerceptronLayer::Weights() const { return outputWeights; }

cl::Buffer PerceptronLayer::Compute(cl::Buffer inputs, std::vector<double> inputWeights, int count) {
	auto queue   = Kernels::Queue();
	auto weights = Kernels::Buffer(1);
	auto biases  = Kernels::Buffer(2);
	auto outputs = Kernels::Buffer(3);

	// Load Buffers
	int ret = queue.enqueueWriteBuffer(weights, CL_TRUE, 0, sizeof(double) * count, inputWeights.data());
	if (ret != CL_SUCCESS) std::cout << "Error Filling Weights buffer. Code " << ret << "\n";
	ret = queue.enqueueWriteBuffer(biases, CL_TRUE, 0, sizeof(double) * bias.size(), bias.data());
	if (ret != CL_SUCCESS) std::cout << "Error Filling Biases buffer. Code " << ret << "\n";

	// Bind Kernel Parameters (Perceptrons)
	auto kernel = Kernels::Perceptron();
	kernel.setArg(0, inputs);
	kernel.setArg(1, weights);
	kernel.setArg(2, count);
	kernel.setArg(3, biases);
	kernel.setArg(4, outputs);

	// Execute Kernel
	ret = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(Depth()));
	if (ret != CL_SUCCESS) std::cout << "Error executing " << Name() << ". Code " << ret << "\n";
	queue.finish();

	// Transfer to buffer 0
	inputs = Kernels::Buffer(0);
	ret    = queue.enqueueCopyBuffer(outputs, inputs, 0, 0, sizeof(double) * bias.size());
	if (ret != CL_SUCCESS) std::cout << "Error executing " << Name() << ". Code " << ret << "\n";

	return inputs;
}

void PerceptronLayer::GenerateRandomWeights() {
	for (int i = 0; i < outputWeights.size(); i++)
		outputWeights[i] = std::rand() / static_cast<double>(INT_MAX);
}
