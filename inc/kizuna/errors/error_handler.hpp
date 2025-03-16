#ifndef ERROR_HANDLER
#define ERROR_HANDLER

#include <queue>
#include <thread>

#include <errors/errors.hpp>

class ErrorManager {
public:
	void Start();
	void Stop();

	static void Queue(const Error& e);

private:
	void Loop();

private:
	bool running;
	std::thread handler;
	static std::queue<Error> errorQueue;
};

#endif /* ERROR_HANDLER */
