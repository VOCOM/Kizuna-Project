#ifndef HARMONY
#define HARMONY

#include <thread>

class Harmony {
public:
	void Start();

	Harmony();
	~Harmony();

private:
	static void Loop();

private:
	static bool running;
	static std::thread mainThread;

	static int cpuThreadMax;
	static int gpuThreadMax;
};

#endif /* HARMONY */
