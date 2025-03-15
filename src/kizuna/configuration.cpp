#include <configuration.hpp>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

std::map<std::string, std::map<std::string, std::string>> Configuration::Config;

void Configuration::ListConfig(std::string filter) {
	std::cout << "Loaded Configuration:\n\n";
	for (auto& modules : Config) {
		if ((filter != "") && (filter != modules.first)) continue;

		std::cout << "Submodule: " << modules.first << "\n";
		int w = 0;
		for (auto& param : modules.second)
			if (param.first.size() > w) w = param.first.size();

		for (auto& param : modules.second) {
			std::cout << std::setw(w + 1) << std::left << param.first;
			std::cout << param.second << "\n";
		}
		std::cout << "\n";
	}
}
void Configuration::LoadConfig() {
	std::string module, paramLine;
	std::ifstream config("config.ini");
	while (!config.eof()) {
		config >> paramLine;
		if (paramLine.empty()) continue;
		std::transform(paramLine.begin(), paramLine.end(), paramLine.begin(), std::tolower);

		// Comment Line
		if (paramLine.front() == ';') continue;

		// Module Header
		if (paramLine.front() == '[') {
			module = paramLine.substr(1, paramLine.size() - 2);
			continue;
		}

		// Extract Parameter
		std::string key, value;
		int delimiter = paramLine.find('=');
		key           = paramLine.substr(0, delimiter);
		value         = paramLine.substr(delimiter + 1, paramLine.size());

		// Store Parameter
		Configuration::Config[module][key] = value;
	}
}
