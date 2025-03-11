#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <utility/timer.hpp>
#include <utility/utils.hpp>

#include <core.hpp>
#include <harmony.hpp>
#include <webserver.hpp>

#include <unsupervised.hpp>

void HelpCommand(std::queue<std::string>& params) {}
void ConfigCommand(std::queue<std::string>& params) {
	std::string filter;
	if (params.size() > 0) filter = params.front();
	Configuration::ListConfig(filter);
}

void SubmoduleCommand(std::queue<std::string>& params, void (Submodule::*function)()) {
	std::string param;
	while (params.size() > 0) {
		param = params.front();
		std::transform(param.begin(), param.end(), param.begin(), std::tolower);
		for (auto& submodule : Kizuna::SubmoduleList) {
			std::string name(submodule->Name());
			std::transform(name.begin(), name.end(), name.begin(), std::tolower);
			if (param != name && param != "all") continue;
			std::bind(function, submodule)();
		}
		params.pop();
		if (param == "all")
			params = std::queue<std::string>();
	}
}

int main(int argc, char** argv) {
	system("cls");
	Configuration::LoadConfig();

	Kizuna::StartErrorHandler();
	Kizuna::LoadSubmodule(std::make_shared<WebServer>());
	Kizuna::LoadSubmodule(std::make_shared<Harmony>());

	bool running = true;
	std::string buffer, command;
	while (running) {
		std::cout << "Kizuna: ";
		std::getline(std::cin, buffer);
		std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);

		auto params = Enqueue(buffer);
		command     = params.front();
		params.pop();

		// Submodule Commands
		if (command == "start") SubmoduleCommand(params, &Submodule::Start);
		if (command == "stop") SubmoduleCommand(params, &Submodule::Stop);
		if (command == "restart") SubmoduleCommand(params, &Submodule::Restart);
		if (command == "info") SubmoduleCommand(params, &Submodule::Info);
		if (command == "link") SubmoduleCommand(params, &Submodule::EnterShell);

		if (command == "help") HelpCommand(params);
		if (command == "config") ConfigCommand(params);
		if (command == "clear") system("cls");
		if (command == "exit") running = false;
	}

	Kizuna::Shutdown();
}