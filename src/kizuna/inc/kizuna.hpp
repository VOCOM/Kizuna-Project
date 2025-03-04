#ifndef KIZUNA
#define KIZUNA

#include <exception>
#include <map>
#include <string>
#include <utility>

#include <CL/opencl.hpp>

class Configuration {
public:
	static void ListConfig(std::string filter = "");
	static void LoadConfig();

	static std::map<std::string, std::map<std::string, std::string>> Config;

	Configuration() = delete;
};

class Kizuna {
public:
private:
};

#endif /* KIZUNA */
