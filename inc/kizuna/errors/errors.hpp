#ifndef ERRORS
#define ERRORS

#include <exception>
#include <sstream>
#include <string>

class Error {
public:
	Error(const char* source, const char* error) : source(source), error(error) {}
	virtual ~Error() {}

public:
	const char* source;
	const char* error;
};
class SoftFault : public Error {
public:
	SoftFault(const char* source, const char* error) : Error(source, error) {}
};
class HardFault : public Error {
public:
	HardFault(const char* source, const char* error) : Error(source, error) {}
};

#endif /* ERRORS */
