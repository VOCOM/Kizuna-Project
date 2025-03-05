#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <utility/timer.hpp>

#include <harmony.hpp>
#include <kizuna.hpp>
#include <webserver.hpp>

std::string GetCommand(std::string& buffer) {
	int idx = buffer.find_first_of(' ');
	return idx != std::string::npos ? buffer.substr(0, idx) : buffer;
}
std::vector<std::string> GetParameters(std::string& buffer) {
	std::vector<std::string> params;
	int idx = buffer.find_last_of(' ');

	while (idx != std::string::npos) {
		params.push_back(buffer.substr(idx + 1, buffer.size()));
		buffer.erase(idx, buffer.size());
		idx = buffer.find_last_of(' ');
	}

	return params;
}

void ConfigCommand(std::vector<std::string>& params) {
	std::string filter;
	if (params.size() > 0) filter = params.back();
	Configuration::ListConfig(filter);
}
void HelpCommand() {}
void InfoCommand(std::vector<std::string>& params) {
	std::string type;
	if (params.size() > 0) type = params.back();
	if (type == "submodules") {
		for (auto& submodule : Kizuna::submodules) {
			std::cout << "\n";
			submodule->Info();
		}
		std::cout << "\n";
	}
}

int main(int argc, char** argv) {
	Configuration::LoadConfig();

	Kizuna::LoadSubmodule(std::make_shared<Harmony>());
	Kizuna::LoadSubmodule(std::make_shared<WebServer>());

	bool running = true;
	std::string buffer, command;
	while (running) {
		std::getline(std::cin, buffer);
		std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);

		command = GetCommand(buffer);
		if (command == "config") ConfigCommand(GetParameters(buffer));
		if (command == "help") HelpCommand();
		if (command == "info") InfoCommand(GetParameters(buffer));
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