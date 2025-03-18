#ifndef INPUT_LAYERS
#define INPUT_LAYERS

#include <abstract_layer.hpp>

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

#endif /* INPUT_LAYERS */
