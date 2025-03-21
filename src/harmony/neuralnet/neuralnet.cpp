#include <neuralnet/neuralnet.hpp>

#include <iostream>

#include <kernels.hpp>
#include <layers.hpp>

void NeuralNet::Info(int count) {
	std::cout << "Neural Network\n"
						<< "Hidden layers: " << layers.size() - 1 << "\n";

	for (int i = 0; i < layers.size(); i++)
		std::cout << "Layer " << i << " Depth " << layers[i]->Depth() << "\n";
}

void NeuralNet::Train(int maxThreads) {
	lock = true;

	layers.push_back(std::make_unique<ClassificationLayer>(data));

	int epoch  = 1;
	auto input = Kernels::Buffer(0);
	for (int i = 0; i < epoch; i++) {
		input = FeedForward(input);

		// # TODO
		input = BackPropagate(input);
	}

	std::vector<double> o(2);
	auto queue = Kernels::Queue();
	queue.enqueueReadBuffer(input, CL_TRUE, 0, sizeof(double) * 2, o.data());
	for (auto d : o) std::cout << d << "\n";

	lock = false;
}

void NeuralNet::AddLayer(std::unique_ptr<Layer> layer) {
	layers.push_back(std::move(layer));
}

NeuralNet::NeuralNet() {
	layers.push_back(std::make_unique<InputLayer>());
}

cl::Buffer NeuralNet::FeedForward(cl::Buffer input) {
	Data::RawData inputData = data.GetData().transpose();

	int count  = data.Cols();
	auto queue = Kernels::Queue();
	int ret    = queue.enqueueWriteBuffer(input, CL_TRUE, 0, sizeof(double) * count, inputData.data());
	if (ret != CL_SUCCESS) std::cout << "Error Filling Input buffer. Code " << ret << "\n";

	int i = 0;
	std::vector<double> weights;
	for (auto& layer : layers) {
		input   = layer->Compute(input, weights, count);
		count   = layer->Depth();
		weights = layer->Weights();
		std::cout << layer->Name() << "\n";
	}

	return input;
}

cl::Buffer NeuralNet::BackPropagate(cl::Buffer input) {
	return input;
}
