#ifndef ERROR_EMITTER
#define ERROR_EMITTER

#include <string>

#include <kizuna/kizuna.hpp>

class ErrorEmitter {
public:
	void Raise(const char* errorMessage) {
		Kizuna::ErrorQueue.push(Error(name, errorMessage));
	}

	ErrorEmitter(const char* name) : name(name) {}

private:
	const char* name;
};

#endif /* ERROR_EMITTER */
