#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <exception>
#include <sstream>
#include <string>

class Error : public std::exception {
public:
	Error() {}
	Error(const char* errMsg, const char* source) : std::exception(errMsg), source(source) {}
	const char* source;
};

#endif /* EXCEPTIONS */
