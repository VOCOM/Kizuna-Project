#include <kizuna.hpp>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <configuration.hpp>

std::vector<std::shared_ptr<Submodule>> Kizuna::SubmoduleList;
std::queue<std::exception> Kizuna::ErrorQueue;
std::map<std::string, std::map<std::string, std::string>> Configuration::Config;

StatusCode Kizuna::errorHandlerStatus;
std::thread Kizuna::errorHandler;

void Configuration::ListConfig(std::string filter) {
	std::cout << "Loaded Configuration:\n\n";
	for (auto& modules : Config) {
		if ((filter != "") && (filter != modules.first)) continue;

		std::cout << "Submodule: " << modules.first << "\n";
		for (auto& param : modules.second) {
			std::cout << std::setw(11) << std::left << param.first;
			std::cout << param.second << "\n";
		}
		std::cout << "\n";
	}
}
void Configuration::LoadConfig() {
	std::string submodule, paramLine;
	std::ifstream config("config.ini");
	while (config.eof() == false) {
		config >> paramLine;
		std::transform(paramLine.begin(), paramLine.end(), paramLine.begin(), std::tolower);

		// Comment Line
		if (paramLine.front() == ';') continue;

		// Module Header
		if (paramLine.front() == '[') {
			submodule = paramLine.substr(1, paramLine.size() - 2);
			continue;
		}

		// Extract Parameter
		std::string key, value;
		int delimiter = paramLine.find('=');
		key           = paramLine.substr(0, delimiter);
		value         = paramLine.substr(delimiter + 1, paramLine.size());

		// Store Parameter
		Configuration::Config[submodule][key] = value;
	}
}

void Kizuna::ErrorHandler() {
	while (errorHandlerStatus != Offline) {
		if (ErrorQueue.size() > 0) {
			auto err = ErrorQueue.front();
			std::cout << err.what();
		} else {
			std::this_thread::yield();
		}
	}
}
void Kizuna::StartErrorHandler() {
	errorHandlerStatus = Online;
	errorHandler       = std::thread(ErrorHandler);
}
void Kizuna::StopErrorHandler() {
	errorHandlerStatus = Offline;
	errorHandler.join();
}

void Kizuna::LoadSubmodule(std::shared_ptr<Submodule> submodule) {
	submodule->LoadConfiguration();
	submodule->Start();
	SubmoduleList.push_back(submodule);
}
void Kizuna::Shutdown() {
	for (auto& submodule : SubmoduleList) {
		submodule->Stop();
		submodule->~Submodule();
	}
	StopErrorHandler();
}

std::string Submodule::Status() const {
	std::string statusString("Offline");
	switch (status) {
	case StatusCode::Online:
		statusString = "Online";
		break;
	case StatusCode::Busy:
		statusString = "Busy";
		break;
	}
	return statusString;
}
