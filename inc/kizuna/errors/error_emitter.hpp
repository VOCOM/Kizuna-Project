#ifndef ERROR_EMITTER
#define ERROR_EMITTER

#include <memory>
#include <string>

#include <errors.hpp>

class ErrorManager;
class Emitter {
public:
	void Raise(std::string errorMessage);
	void Assert(std::string errorMessage);

	Emitter(const char* name) : name(name) {}

public:
	void AddToQueue(const Error error);

private:
	const char* name;
};

#endif /* ERROR_EMITTER */
