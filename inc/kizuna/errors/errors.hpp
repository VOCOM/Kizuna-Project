#ifndef ERRORS
#define ERRORS

#include <exception>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>

class Error {
public:
	Error(const char* source, std::string error) : source(source), error(error) {}
	virtual ~Error() {}

public:
	const char* source;
	std::string error;
};
class SoftFault : public Error {
public:
	SoftFault(const char* source, std::string error) : Error(source, error) {}
};
class HardFault : public Error {
public:
	HardFault(const char* source, std::string error) : Error(source, error) {}
};

class ErrorQueue {
public:
	bool empty() {
		std::unique_lock<std::mutex> lock(m);
		return q.empty();
	}
	void push(Error item) {
		std::unique_lock<std::mutex> lock(m);
		q.push(item);
		c.notify_one();
	}
	Error pop() {
		std::unique_lock<std::mutex> lock(m);
		c.wait(lock, [this]() { return !q.empty(); });
		Error item = q.front();
		q.pop();
		return item;
	}

private:
	std::mutex m;
	std::queue<Error> q;
	std::condition_variable c;
};

#endif /* ERRORS */
