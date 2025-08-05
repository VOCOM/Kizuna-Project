#include <configuration.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "utility/utils.hpp"

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
	std::string configPath(std::filesystem::current_path().string());
	configPath.append("\\config.ini");

	std::string module, paramLine;
	std::fstream config;

	config.open(configPath, std::ios::in | std::ios::out);
	while (std::getline(config, paramLine)) {
		if (paramLine.empty()) continue;
		paramLine = ToLower(paramLine);

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
	config.close();
}
