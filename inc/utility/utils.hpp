#ifndef UTILS
#define UTILS

#include <queue>
#include <string>
#include <vector>

bool IsNumerical(std::string& input);
void ReplaceToken(std::string& input, const char delim, const char c = ' ');
std::vector<std::string> Split(std::string& input, const char delim = ',');
std::queue<std::string> Enqueue(std::string& buffer);
int Digits(int number);

int ToInt(std::string input);
std::string ToLower(std::string input);

bool Contains(std::string haystack, const char* needle);
bool Contains(std::string haystack, std::string needle);

#endif /* UTILS */
