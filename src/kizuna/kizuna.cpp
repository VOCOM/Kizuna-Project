#include <kizuna.hpp>

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <configuration.hpp>
#include <errors/error_emitter.hpp>
#include <utility/utils.hpp>

void Kizuna::Access() {
	std::string command;

	while (true) {
		std::cout << "Kizuna: ";
		std::cin >> buffer;
		buffer >> command;

		// Module Commands
		if (command == "start") ModuleCommand(&Module::Start);
		if (command == "stop") ModuleCommand(&Module::Stop);
		if (command == "restart") ModuleCommand(&Module::Restart);
		if (command == "info") ModuleCommand(&Module::Info);
		if (command == "link") ModuleCommand(&Module::Access);

		if (command == "help") HelpCommand();
		if (command == "config") ConfigCommand();
		if (command == "clear") Clear();
		if (command == "exit") return;
	}
}

void Kizuna::Initialize() {
	Clear();
	Configuration::LoadConfig();
}
void Kizuna::Shutdown() {
	Module::Shutdown();
}
void Kizuna::LoadModule(const std::shared_ptr<Module>& module) {
	module->LoadConfiguration();
	module->Start();
	module->RegisterModule(module);
}

Kizuna::~Kizuna() {
	Shutdown();
}

void Kizuna::HelpCommand() {
	std::cout << "\nShell Commands\n"
						<< std::setw(10) << std::left << "CONFIG" << "Display configuration.\n"
						<< std::setw(10) << std::left << "CLEAR" << "Clear terminal screen.\n"
						<< std::setw(10) << std::left << "HELP" << "List commands.\n"
						<< std::setw(10) << std::left << "EXIT" << "Shutdown application.\n"
						<< "\nSubmodule Commands\n"
						<< std::setw(10) << std::left << "START" << "Startup a submodule.\n"
						<< std::setw(10) << std::left << "STOP" << "Shutdown a submodule.\n"
						<< std::setw(10) << std::left << "RESTART" << "Restart a submodule.\n"
						<< std::setw(10) << std::left << "INFO" << "Display submodule information.\n"
						<< std::setw(10) << std::left << "LINK" << "Enter submodule shell.\n"
						<< "\n";
}
void Kizuna::ConfigCommand() {
	std::string filter;
	if (buffer.size() > 0) filter = buffer.pop();
	Configuration::ListConfig(filter);
}
void Kizuna::ModuleCommand(void (Module::*function)()) {
	if (buffer.empty()) return;
	std::string param = buffer.pop();

	for (auto& module : Module::GetModules()) {
		std::string name = module->Name();
		std::transform(name.begin(), name.end(), name.begin(), std::tolower);
		if (param != "all" && param != name) continue;
		std::bind(function, module)();
	}
	std::cout << "\n";
}
