#ifndef HARMONY
#define HARMONY

#include <queue>
#include <thread>
#include <utility>

#include <database/database_accessor.hpp>
#include <errors/error_emitter.hpp>
#include <kizuna/module.hpp>
#include <utility/utils.hpp>

#include <kernels.hpp>
#include <model.hpp>

class Harmony : public Module, public DatabaseAccessor {
public:
	// Module Interface
	virtual std::string Name() { return "Harmony"; }
	virtual std::string Status() { return ToString(status); }
	virtual void Info();
	virtual void Start();
	virtual void Stop();
	virtual void Restart();
	virtual void LoadConfiguration();

	// Shell Interface
	virtual void Access();
	void CommandList();

	// Neural Net
	void CreateNeuralNet();

	Harmony();
	virtual ~Harmony();

private:
	void Loop();

private:
	int maxCpuThreads;
	int maxGpuThreads;

	StatusCode status;
	std::atomic_bool lock;
	std::thread mainThread;
	std::vector<std::thread> workerThreads;

	Data holdingData;
	std::shared_ptr<Model> currentModel;
	std::queue<std::shared_ptr<Model>> runQueue;
	std::vector<std::shared_ptr<Model>> completedQueue;
};

#endif /* HARMONY */
