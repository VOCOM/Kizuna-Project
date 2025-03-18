#ifndef NEURALNET
#define NEURALNET

#include <memory>

#include <abstract_layer.hpp>
#include <model.hpp>

class NeuralNet : public Model {
public:
	virtual std::string Name() { return "NeuralNet"; }
	virtual void Load(Data& newData) { data = Data(newData); }
	virtual void Info(int count = -1);

	virtual void Train(int maxThreads);
	virtual void Test(int maxThreads) {}

	virtual Results Result() { return Results(); }

	void AddLayer(int depth);

	NeuralNet();

private:
	cl::Buffer FeedForward(cl::Buffer input);
	cl::Buffer BackPropagate(cl::Buffer input);

private:
	std::atomic_bool lock;
	std::vector<std::unique_ptr<Layer>> layers;
	Data data;
};

#endif /* NEURALNET */
