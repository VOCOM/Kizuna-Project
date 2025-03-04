#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <timer.hpp>

#include <harmony.hpp>
#include <kizuna.hpp>
#include <unsupervised.hpp>
#include <webserver.hpp>

void ConfigCommand(std::string& buffer) {
	std::string filter;
	std::vector<std::string> params;

	Configuration::ListConfig(filter);
}
void HelpCommand() {
}

int main(int argc, char** argv) {
	if (Kizuna::Init() == false) {
		std::cout << "Failed to initialize OpenCL\n";
		return 1;
	}

	WebServer server;
	server.Start();

	Harmony harmony;
	harmony.Start();

	bool running = true;
	std::string buffer;
	while (running) {
		std::getline(std::cin, buffer);
		std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);

		if (!buffer.find("config")) ConfigCommand(buffer);
		if (buffer == "help") HelpCommand();
		if (buffer == "exit") running = false;
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