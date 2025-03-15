#ifndef SUPERVISED
#define SUPERVISED

#include <data.hpp>
#include <model.hpp>
#include <results.hpp>

class KNN : public Model {
public:
	virtual std::string Name() { return "KNN"; }
	virtual void Load(DataTable& newData) { data = DataTable(newData); }
	virtual void Info(int count = -1);

	virtual void Train(int maxThreads);
	virtual void Test(int maxThreads);

	virtual Results Result();

public:
	int k;
	std::atomic_bool lock;
	DataTable data;
};

#endif /* SUPERVISED */
