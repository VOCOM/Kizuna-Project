#ifndef SHELL
#define SHELL

#include <queue>
#include <string>

#include <utility/utils.hpp>

class Shell {
public:
	std::queue<std::string> GetParameters(std::string& buffer) {
		auto paramList = Split(buffer, ' ');
		std::queue<std::string> params;
		for (auto& param : paramList) params.push(param);
		return params;
	}

private:
};

#endif /* SHELL */
