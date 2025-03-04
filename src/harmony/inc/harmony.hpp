#ifndef HARMONY
#define HARMONY

#include <thread>
#include <utility>

#include <CL/opencl.hpp>

class Harmony {
public:
	void Start();

	static cl::Buffer& Buffer(int idx);
	static cl::CommandQueue& Queue() { return queue; }

	static cl::Kernel& Distance() { return euclid; }
	static cl::Kernel& Centroid() { return centroid; }

	Harmony();
	~Harmony();

private:
	static void Loop();

	static void LoadConfiguration();
	static bool LoadPlatform();
	static bool LoadDevice(uint8_t cl_device_type);
	static bool BuildProgram(cl::Context& context, cl::Device& device, std::string source_path, cl::Program& program);

private:
	static bool running;
	static std::thread mainThread;

	static int MAX_CPU_THREADS;
	static int MAX_GPU_THREADS;
	static int MAX_BUFFER_COUNT;
	static int MAX_BUFFER_SIZE;

	static std::vector<cl::Buffer> buffers;
	static cl::CommandQueue queue;
	static cl::Kernel euclid;
	static cl::Kernel centroid;
};

#endif /* HARMONY */
