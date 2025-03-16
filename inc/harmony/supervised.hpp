#ifndef SUPERVISED
#define SUPERVISED

#include <database/data.hpp>
#include <model.hpp>
#include <results.hpp>

class KNN : public Model {
public:
	virtual std::string Name() { return "KNN"; }
	virtual void Load(Data& newData) { data = Data(newData); }
	virtual void Info(int count = -1);

	virtual void Train(int maxThreads);
	virtual void Test(int maxThreads);

	virtual Results Result();

public:
	int k;
	std::atomic_bool lock;
	Data data;
};

#endif /* SUPERVISED */
