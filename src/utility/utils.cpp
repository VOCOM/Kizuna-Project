#include <utils.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>

using namespace std::placeholders;

char delimiter, token;
void ReplaceToken(std::string& input, const char delim, const char c) {
	delimiter = delim;
	token     = c;
	std::transform(input.begin(), input.end(), input.begin(), [](char& c) { return c == delimiter ? token : c; });
}

int sum = 0;
bool IsNumerical(std::string& input) {
	sum = 0;
	std::for_each(input.begin(), input.end(), [](char& c) { sum += std::isalpha(c); });
	return !sum;
}

std::vector<std::string> Split(std::string& input, const char delim) {
	ReplaceToken(input, delim);
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
