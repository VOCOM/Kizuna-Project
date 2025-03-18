#ifndef OUTPUT_LAYERS
#define OUTPUT_LAYERS

#include <abstract_layer.hpp>

#include <database/data.hpp>

class ClassificationLayer : public Layer {
public:
	virtual std::string Name() const { return "Classification Layer"; }
	virtual int Depth() const { return depth; }
	virtual std::vector<double> Weights() const { return std::vector<double>(depth); }

	virtual cl::Buffer Compute(cl::Buffer input, std::vector<double> inputWeights, int count) {
		return input;
	}

	ClassificationLayer(Data& data) {
		Data::Header labels = data.GetLabel();
		std::unordered_map<std::string, int> map;
		for (auto& label : labels) map[label]++;
		for (auto& kv : map) depth++;
		prediction.resize(depth);
	}

private:
	int depth = 0;
	std::vector<int> truth;
	std::vector<int> prediction;
};

#endif /* OUTPUT_LAYERS */
