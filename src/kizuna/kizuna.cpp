#include <kizuna.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

#include <configuration.hpp>
#include <database/database_accessor.hpp>
#include <errors/error_emitter.hpp>
#include <utility/utils.hpp>

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
	errorHandler.Start();
}
void Kizuna::Shutdown() {
	Module::Shutdown();
	errorHandler.Stop();
}
void Kizuna::LoadModule(const std::shared_ptr<Module>& module) {
	module->LoadConfiguration();
	module->Start();
	Module::GetModules().push_back(module);
}

Kizuna::~Kizuna() {
	Shutdown();
}

void Kizuna::HelpCommand(std::queue<std::string>& params) {}
void Kizuna::ConfigCommand(std::queue<std::string>& params) {
	std::string filter;
	if (params.size() > 0) filter = params.front();
	Configuration::ListConfig(filter);
}
void Kizuna::ModuleCommand(std::queue<std::string>& params, void (Module::*function)()) {
	std::string param;

	if (params.empty()) return;
	param = params.front();
	params.pop();
	std::transform(param.begin(), param.end(), param.begin(), std::tolower);

	if (param == "all") {
		for (auto module : Module::GetModules())
			std::bind(function, module)();
		return;
	}

	auto module = Module::GetModule(param);
	std::bind(function, module)();
}
