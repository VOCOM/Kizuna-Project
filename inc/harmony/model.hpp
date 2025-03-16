#ifndef MODEL
#define MODEL

#include <database/data.hpp>
#include <results.hpp>

class Model {
public:
	virtual std::string Name()        = 0;
	virtual void Load(Data& newData)  = 0;
	virtual void Info(int count = -1) = 0;

	virtual void Train(int maxThreads) = 0;
	virtual void Test(int maxThreads)  = 0;

	virtual Results Result() = 0;

	virtual ~Model() {}
};

#endif /* MODEL */
