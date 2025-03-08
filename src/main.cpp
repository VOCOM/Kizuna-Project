#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <utility/timer.hpp>
#include <utility/utils.hpp>

#include <core.hpp>
#include <harmony.hpp>
#include <webserver.hpp>

std::queue<std::string> GetParameters(std::string& buffer) {
	auto paramList = Split(buffer, ' ');
	std::queue<std::string> params;
	for (auto& param : paramList) params.push(param);
	return params;
}

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
			if (param != name) continue;
			std::bind(function, submodule)();
		}
		params.pop();
	}
}

int main(int argc, char** argv) {
	system("cls");
	Configuration::LoadConfig();

	Kizuna::StartErrorHandler();
	Kizuna::LoadSubmodule(std::make_shared<Harmony>());
	Kizuna::LoadSubmodule(std::make_shared<WebServer>());

	bool running = true;
	std::string buffer, command;
	while (running) {
		std::cout << "Kizuna: ";
		std::getline(std::cin, buffer);
		std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);

		auto params = GetParameters(buffer);
		command     = params.front();
		params.pop();

		// Submodule Commands
		if (command == "start") SubmoduleCommand(params, &Submodule::Start);
		if (command == "stop") SubmoduleCommand(params, &Submodule::Stop);
		if (command == "restart") SubmoduleCommand(params, &Submodule::Restart);
		if (command == "info") SubmoduleCommand(params, &Submodule::Info);

		if (command == "help") HelpCommand(params);
		if (command == "config") ConfigCommand(params);
		if (command == "clear") system("cls");
		if (command == "exit") running = false;
	}

	Kizuna::Shutdown();

	// Timer timer;
	// DataTable data;
	// int c = 10000;
	// std::vector<double> feature1, feature2, feature3, feature4;
	// for (int i = 0; i < c; i++) {
	// 	feature1.push_back(std::rand() % 99 + 0.1);
	// 	feature2.push_back(std::rand() % 99 + 0.1);
	// 	feature3.push_back(std::rand() % 99 + 0.1);
	// 	feature4.push_back(std::rand() % 99 + 0.1);
	// }
	// data.AddFeature("Feature 1", feature1);
	// data.AddFeature("Feature 2", feature2);
	// data.AddFeature("Feature 3", feature3);
	// data.AddFeature("Feature 4", feature4);
	// // data.Info();
	// std::cout << "\nElement Count: " << c << "\n";
	// timer.Start();
	// Clusters clusters = KMeans(4, data);
	// timer.Stop();
	// std::cout << "GPU KMeans: " << timer.DurationS() << "s\n\n";
}