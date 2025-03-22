#ifndef ERROR_HANDLER
#define ERROR_HANDLER

#include <thread>

#include <errors/error_emitter.hpp>

class ErrorManager {
public:
	ErrorManager();
	~ErrorManager();

private:
	void Loop() const;

private:
	bool running;
	std::thread handler;

	static ErrorQueue errorQueue;

	friend void Emitter::AddToQueue(const Error error);
};

#endif /* ERROR_HANDLER */
