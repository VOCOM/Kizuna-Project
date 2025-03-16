#ifndef ERROR_HANDLER
#define ERROR_HANDLER

#include <queue>
#include <thread>

#include <errors/error_emitter.hpp>

class ErrorManager {
public:
	void Start();
	void Stop();

private:
	void Loop();

private:
	bool running;
	std::thread handler;

	std::atomic_bool queueLock;
	std::queue<Error> errorQueue;

	static std::shared_ptr<ErrorManager> instance;

	friend void Emitter::AddToQueue(const Error& error);
};

#endif /* ERROR_HANDLER */
