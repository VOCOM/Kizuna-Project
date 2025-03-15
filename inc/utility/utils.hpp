#ifndef UTILS
#define UTILS

#include <queue>
#include <string>
#include <vector>

#include <kizuna/status.hpp>

std::vector<std::string> Split(std::string& input, const char delim);
std::queue<std::string> Enqueue(std::string& buffer);
int Digits(int number);

std::string ToString(StatusCode& status);

#endif /* UTILS */
