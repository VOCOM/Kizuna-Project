#include "utils.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>

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

std::queue<std::string> Enqueue(std::string& buffer) {
	auto paramList = Split(buffer, ' ');
	std::queue<std::string> params;
	for (auto& param : paramList) params.push(param);
	return params;
}

int Digits(int number) {
	int digits = 1;
	while (number > 0) {
		number /= 10;
		digits++;
	}
	return digits;
}

std::string ToString(StatusCode& status) {
	return status == Online    ? "Online"
	       : status == Busy    ? "Busy"
	       : status == Faulted ? "Faulted"
	                           : "Offline";
}
