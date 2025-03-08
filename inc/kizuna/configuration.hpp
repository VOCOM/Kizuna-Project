#ifndef CONFIGURATION
#define CONFIGURATION

#include <map>
#include <string>

class Configuration {
public:
	static void ListConfig(std::string filter = "");
	static void LoadConfig();

	static std::map<std::string, std::map<std::string, std::string>> Config;

	Configuration() = delete;
};

#endif /* CONFIGURATION */
