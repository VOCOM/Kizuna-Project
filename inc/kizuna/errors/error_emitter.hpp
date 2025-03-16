#ifndef ERROR_EMITTER
#define ERROR_EMITTER

#include <string>

#include <errors/error_handler.hpp>
#include <kizuna.hpp>

class Emitter {
public:
	void Raise(std::string errorMessage) {
		Raise(errorMessage.c_str());
	}
	void Raise(const char* errorMessage) {
		ErrorManager::Queue(SoftFault(name, errorMessage));
	}

	void Assert(std::string errorMessage) {
		Assert(errorMessage.c_str());
	}
	void Assert(const char* errorMessage) {
		ErrorManager::Queue(HardFault(name, errorMessage));
	}

	Emitter(const char* name) : name(name) {}

private:
	const char* name;
};

#endif /* ERROR_EMITTER */
