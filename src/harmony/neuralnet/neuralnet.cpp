#include <neuralnet/neuralnet.hpp>

#include <iostream>

#include <harmony.hpp>

void NeuralNet::Info(int count) {
	std::cout << "Neural Network\n"
						<< "Hidden layers: " << layers.size() << "\n";
	for (int i = 0; i < layers.size(); i++)
		std::cout << "Layer " << i << " Depth " << layers[i]->Depth() << "\n";
}

void NeuralNet::Train(int maxThreads) {
	lock = true;

	int epoch = 1;
	for (int i = 0; i < epoch; i++) {
		FeedForward();
		BackPropagate();
	}

	lock = false;
}

void NeuralNet::AddLayer(int depth) {
	layers.push_back(std::make_unique<ReLuLayer>(depth));
}

NeuralNet::NeuralNet() {
	layers.push_back(std::make_unique<InputLayer>());
}

void NeuralNet::FeedForward() {
	DataTable::Data inputData = data.GetData().transpose();

	int count  = data.Cols();
	auto queue = Harmony::Queue();
	auto input = Harmony::Buffer(0);
	int ret    = queue.enqueueWriteBuffer(input, CL_TRUE, 0, sizeof(double) * count, inputData.data());
	if (ret != CL_SUCCESS) std::cout << "Error Filling Input buffer. Code " << ret << "\n";

	int i = 0;
	std::vector<double> weights;
	for (auto& layer : layers) {
		input   = layer->Compute(input, weights, count);
		count   = layer->Depth();
		weights = layer->Weights();
	}
}

void NeuralNet::BackPropagate() {
}
