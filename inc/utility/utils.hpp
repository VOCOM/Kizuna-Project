#ifndef UTILS
#define UTILS

#include <queue>
#include <string>
#include <vector>

std::vector<std::string> Split(std::string& input, const char delim);
std::queue<std::string> Enqueue(std::string& buffer);
int Digits(int number);

#endif /* UTILS */
