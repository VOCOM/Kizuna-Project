#include <kizuna.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

#include <configuration.hpp>

std::vector<std::shared_ptr<Module>> Kizuna::ModuleList;
std::queue<Error> Kizuna::ErrorQueue;

void Kizuna::LoadModule(std::shared_ptr<Module> module) {
	module->LoadConfiguration();
	module->Start();
	ModuleList.push_back(module);
}

void Kizuna::Access() {
	std::string buffer, command;
	std::queue<std::string> params;

	while (true) {
		std::cout << "Kizuna: ";
		std::getline(std::cin, buffer);
		std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);

		params  = Enqueue(buffer);
		command = params.front();
		params.pop();

		// Module Commands
		if (command == "start") ModuleCommand(params, &Module::Start);
		if (command == "stop") ModuleCommand(params, &Module::Stop);
		if (command == "restart") ModuleCommand(params, &Module::Restart);
		if (command == "info") ModuleCommand(params, &Module::Info);
		if (command == "link") ModuleCommand(params, &Module::Access);

		if (command == "help") HelpCommand(params);
		if (command == "config") ConfigCommand(params);
		if (command == "clear") Clear();
		if (command == "exit") return;
	}
}

void Kizuna::Initialize() {
	Clear();
	Configuration::LoadConfig();

	errorHandlerStatus = Online;
	errorHandler       = std::thread(&Kizuna::ErrorHandler, this);
}
void Kizuna::Shutdown() {
	for (auto& module : ModuleList) {
		module->Stop();
		module->~Module();
	}

	errorHandlerStatus = Offline;
	errorHandler.join();
}

void Kizuna::HelpCommand(std::queue<std::string>& params) {
}
void Kizuna::ConfigCommand(std::queue<std::string>& params) {
	std::string filter;
	if (params.size() > 0) filter = params.front();
	Configuration::ListConfig(filter);
}
void Kizuna::ModuleCommand(std::queue<std::string>& params, void (Module::*function)()) {
	std::string param;
	while (params.size() > 0) {
		param = params.front();
		std::transform(param.begin(), param.end(), param.begin(), std::tolower);
		for (auto& module : Kizuna::ModuleList) {
			std::string name(module->Name());
			std::transform(name.begin(), name.end(), name.begin(), std::tolower);
			if (param != name && param != "all") continue;
			std::bind(function, module)();
		}
		params.pop();
		if (param == "all")
			params = std::queue<std::string>();
	}
}

void Kizuna::ErrorHandler() {
	while (errorHandlerStatus != Offline) {
		if (ErrorQueue.empty()) {
			std::this_thread::yield();
			continue;
		}

		auto err = ErrorQueue.front();
		for (auto& module : Kizuna::ModuleList) {
			std::string name(module->Name());
			if (err.source != module->Name()) continue;
			std::cout << "\n"
								<< err.source << " faulted\n"
								<< err.error << "\n"
								<< "Status " << module->Status() << "\n";
		}
		ErrorQueue.pop();
	}
}
