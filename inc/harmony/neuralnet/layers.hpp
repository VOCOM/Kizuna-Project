#ifndef LAYERS
#define LAYERS

#include <string>
#include <vector>

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <cl/opencl.hpp>

#include <data.hpp>

class Layer {
public:
	virtual std::string Name() const            = 0;
	virtual int Depth() const                   = 0;
	virtual std::vector<double> Weights() const = 0;

	virtual cl::Buffer Compute(cl::Buffer input, std::vector<double> inputWeights, int count) = 0;

	virtual ~Layer() {}
};

class InputLayer : public Layer {
public:
	virtual std::string Name() const { return "Input Layer"; }
	virtual int Depth() const { return depth; }
	virtual std::vector<double> Weights() const { return std::vector<double>(depth, 1); }

	virtual cl::Buffer Compute(cl::Buffer input, std::vector<double> inputWeights, int count) {
		depth = count;
		return input;
	}

private:
	int depth;
};
class ClassificationLayer : public Layer {
public:
	virtual std::string Name() const { return "Classification Layer"; }
	virtual int Depth() const { return depth; }
	virtual std::vector<double> Weights() const { return std::vector<double>(depth); }

	virtual cl::Buffer Compute(cl::Buffer input, std::vector<double> inputWeights, int count) {
		return input;
	}

	ClassificationLayer(DataTable& data) {
		depth = data.Count();
		// truth = data.Labels;
	}

private:
	int depth;
	std::vector<int> truth;
	std::vector<int> prediction;
};

class ReLuLayer : public Layer {
public:
	virtual std::string Name() const { return "ReLu Layer"; }
	virtual int Depth() const { return bias.size(); }
	virtual std::vector<double> Weights() const { return outputWeights; }

	virtual cl::Buffer Compute(cl::Buffer input, std::vector<double> inputWeights, int count);

	ReLuLayer(int depth) : bias(depth), outputWeights(depth) {}

private:
	std::vector<double> bias;
	std::vector<double> outputWeights;
};

#endif /* LAYERS */
