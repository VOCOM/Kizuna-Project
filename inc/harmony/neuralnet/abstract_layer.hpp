#ifndef ABSTRACT_LAYER
#define ABSTRACT_LAYER

#include <string>
#include <vector>

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <cl/opencl.hpp>

// Abstract Layer
class Layer {
public:
	virtual std::string Name() const            = 0;
	virtual int Depth() const                   = 0;
	virtual std::vector<double> Weights() const = 0;

	virtual cl::Buffer Compute(cl::Buffer input, std::vector<double> inputWeights, int count) = 0;

	virtual ~Layer() {}
};

#endif /* ABSTRACT_LAYER */
