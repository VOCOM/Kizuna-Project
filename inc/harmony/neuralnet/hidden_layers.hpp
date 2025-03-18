#ifndef HIDDEN_LAYERS
#define HIDDEN_LAYERS

#include <abstract_layer.hpp>

// Multi Perceptron Layer
class PerceptronLayer : public Layer {
public:
	virtual int Depth() const;
	virtual std::vector<double> Weights() const;
	virtual cl::Buffer Compute(cl::Buffer input, std::vector<double> inputWeights, int count);
	virtual void GenerateRandomWeights();

	PerceptronLayer(int depth) : bias(depth), outputWeights(depth) {}

private:
	std::vector<double> bias;
	std::vector<double> outputWeights;
};

// Recti-Linear Unit Layer
class ReLuLayer : public PerceptronLayer {
public:
	virtual std::string Name() const { return "ReLu Layer"; }
	virtual cl::Buffer Compute(cl::Buffer input, std::vector<double> inputWeights, int count);

	ReLuLayer(int depth) : PerceptronLayer(depth) {}
};

#endif /* HIDDEN_LAYERS */
