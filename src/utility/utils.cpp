#include "utils.hpp"

#include <algorithm>
#include <functional>
#include <sstream>

#include <iostream>

using namespace std::placeholders;

struct Replace {
	char operator()(char& c) { return c == delimiter ? ' ' : c; }
	static char delimiter;
};
char Replace::delimiter;

std::vector<std::string> Split(std::string& input, const char delim) {
	Replace::delimiter = delim;
	std::transform(input.begin(), input.end(), input.begin(), Replace());
	std::stringstream ss(input);

	std::string param;
	std::vector<std::string> substrings;
	while (!ss.eof()) {
		ss >> param;
		substrings.push_back(param);
	}

	return substrings;
}
