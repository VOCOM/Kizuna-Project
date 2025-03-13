#ifndef UNSUPERVISED
#define UNSUPERVISED

#include <data.hpp>
#include <model.hpp>
#include <results.hpp>

class KMeansc : public Model {
public:
	virtual void Execute();

	KMeansc() : Model("KMeans") {}

public:
	int k;
};

#endif /* UNSUPERVISED */
