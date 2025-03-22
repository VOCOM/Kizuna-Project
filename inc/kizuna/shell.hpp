#ifndef SHELL
#define SHELL

#include <algorithm>
#include <queue>
#include <string>

#include <utility/utils.hpp>

class InputBuffer {
public:
	size_t size() const { return buffer.size(); }
	void clear() {
		while (!buffer.empty()) buffer.pop();
	}

	std::string pop() {
		std::string front(buffer.front());
		buffer.pop();
		return front;
	}

	InputBuffer& operator>>(std::string& output) {
		output = buffer.front();
		buffer.pop();
		return *this;
	}
	InputBuffer& operator<<(std::string& input) {
		buffer.push(input);
		return *this;
	}

	friend std::istream& operator>>(std::istream& is, InputBuffer& buf) {
		std::string line;
		std::getline(is, line);
		std::transform(line.begin(), line.end(), line.begin(), std::tolower);
		auto list = Split(line);
		buf.clear();
		for (std::string& p : list) buf << p;
		return is;
	}
	friend std::ostream& operator<<(std::ostream& os, InputBuffer& buf) {
		os << buf;
		return os;
	}

private:
	std::queue<std::string> buffer;
};

class Shell {
public:
	virtual void Access() = 0;

	virtual void Clear() final { system("cls"); }

	InputBuffer buffer;
};

#endif /* SHELL */
