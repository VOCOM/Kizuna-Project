#ifndef HARMONY
#define HARMONY

#include <queue>
#include <thread>
#include <utility>

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

#include <kizuna/data.hpp>
#include <kizuna/kizuna.hpp>
#include <results.hpp>

class Harmony : public Submodule {
public:
	// Submodule Interface
	virtual void Info();
	virtual void Start();
	virtual void Stop();
	virtual void Restart();
	virtual void LoadConfiguration();

	virtual void ShellHeader();
	virtual void Shell(std::string command, std::queue<std::string> params);

	// Hardware Resources
	static cl::Buffer& Buffer(int idx);
	static cl::CommandQueue& Queue() { return hardwareQueue; }

	// Hardware Functions
	static cl::Kernel& Distance() { return euclid; }
	static cl::Kernel& Centroid() { return centroid; }

	Harmony();
	virtual ~Harmony();

private:
	void Loop();

	static cl::Context GetContext() { return cl::Context(cl::Device::getDefault()); }
	static bool LoadPlatform();
	static bool LoadDevice(uint8_t cl_device_type);
	static bool BuildProgram(cl::Context& context, cl::Device& device, std::string source_path, cl::Program& program);

private:
	static std::thread mainThread;
	static std::vector<std::thread> workerThreads;

	static int MAX_CPU_THREADS;
	static int MAX_GPU_THREADS;
	static int MAX_BUFFER_COUNT;
	static int MAX_BUFFER_SIZE;

	static std::vector<cl::Buffer> buffers;
	static cl::CommandQueue hardwareQueue;
	static cl::Kernel euclid;
	static cl::Kernel centroid;

	static std::atomic_bool dataLock;
	static DataTable data;
	static Results results;
	static std::queue<Results (*)(DataTable&, int)> runQueue;
};

#endif /* HARMONY */
