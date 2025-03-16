#ifndef UNSUPERVISED
#define UNSUPERVISED

#include <database/data.hpp>
#include <model.hpp>
#include <results.hpp>

class KMeans : public Model {
public:
	virtual std::string Name() { return "Kmeans"; }
	virtual void Load(Data& newData) { data = Data(newData); }
	virtual void Info(int count = -1);

	virtual void Train(int maxThreads);
	virtual void Test(int maxThreads);

	virtual Results Result();

public:
	int k;
	std::atomic_bool lock;
	Data data;
	Clusters clusters;
};

#endif /* UNSUPERVISED */
