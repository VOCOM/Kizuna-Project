#ifndef HARMONY
#define HARMONY

#include <queue>
#include <thread>
#include <utility>

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

#include <kizuna/error_emitter.hpp>
#include <kizuna/module.hpp>
#include <utility/utils.hpp>

#include <data.hpp>
#include <model.hpp>

class Harmony : public Module, public ErrorEmitter {
public:
	// Submodule Interface
	virtual std::string Name() { return "Harmony"; }
	virtual std::string Status() { return ToString(status); }
	virtual void Info();
	virtual void Start();
	virtual void Stop();
	virtual void Restart();
	virtual void LoadConfiguration();

	// Kernel Interface
	virtual void Access();

	// Hardware Resources
	static cl::Buffer& Buffer(int idx);
	static cl::CommandQueue& Queue() { return hardwareQueue; }

	// Hardware Functions
	static cl::Kernel& Distance() { return euclid; }
	static cl::Kernel& Centroid() { return centroid; }
	static cl::Kernel& Perceptron() { return perceptron; }
	static cl::Kernel& ReLu() { return relu; }

	Harmony();
	virtual ~Harmony();

private:
	void Loop();

	bool LoadPlatform();
	bool LoadDevice(uint8_t cl_device_type);
	bool BuildProgram(cl::Context& context, cl::Device& device, std::string source_path, cl::Program& program);

private: // Global Variables
	static int MAX_BUFFER_COUNT;
	static int MAX_BUFFER_SIZE;

	static cl::Device device;
	static cl::Context context;
	static std::vector<cl::Buffer> buffers;
	static cl::CommandQueue hardwareQueue;
	static cl::Kernel euclid;
	static cl::Kernel centroid;
	static cl::Kernel perceptron;
	static cl::Kernel relu;

private:
	int maxCpuThreads;
	int maxGpuThreads;

	StatusCode status;
	std::atomic_bool lock;
	std::thread mainThread;
	std::vector<std::thread> workerThreads;

	DataTable holdingData;
	std::shared_ptr<Model> currentModel;
	std::queue<std::shared_ptr<Model>> runQueue;
	std::vector<std::shared_ptr<Model>> completedQueue;
};

#endif /* HARMONY */
