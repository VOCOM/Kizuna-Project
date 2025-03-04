#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <timer.hpp>

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

void ConfigCommand(std::string& buffer) {
	std::string filter;
	auto params = GetParameters(buffer);
	if (params.size() > 0) filter = params.back();
	Configuration::ListConfig(filter);
}
void HelpCommand() {
}

int main(int argc, char** argv) {
	Configuration::LoadConfig();

	WebServer server;
	server.Start();

	Harmony harmony;
	harmony.Start();

	bool running = true;
	std::string buffer, command;
	while (running) {
		std::getline(std::cin, buffer);
		std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);

		command = GetCommand(buffer);
		if (command == "config") ConfigCommand(buffer);
		if (command == "help") HelpCommand();
		if (command == "exit") running = false;
	}
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