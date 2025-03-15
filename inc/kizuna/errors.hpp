#ifndef ERRORS
#define ERRORS

#include <exception>
#include <sstream>
#include <string>

class Error {
public:
	Error(const char* source, const char* error) : source(source), error(error) {}
	const char* source;
	const char* error;
};

#endif /* ERRORS */
