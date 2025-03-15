#ifndef NEURALNET
#define NEURALNET

#include <memory>

#include <model.hpp>
#include <neuralnet/layers.hpp>

class NeuralNet : public Model {
public:
	virtual std::string Name() { return "NeuralNet"; }
	virtual void Load(DataTable& newData) { data = DataTable(newData); }
	virtual void Info(int count = -1);

	virtual void Train(int maxThreads);
	virtual void Test(int maxThreads) {}

	virtual Results Result() { return Results(); }

	void AddLayer(int depth);

	NeuralNet();

private:
	void FeedForward();
	void BackPropagate();

private:
	std::atomic_bool lock;
	std::vector<std::unique_ptr<Layer>> layers;
	DataTable data;
};

#endif /* NEURALNET */
