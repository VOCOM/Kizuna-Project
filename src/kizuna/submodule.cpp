#include <submodule.hpp>

#include <iostream>

#include <utility/utils.hpp>

void Submodule::EnterShell() {
	system("cls");
	bool inContext = true;
	std::string buffer, command;
	while (inContext) {
		std::cout << name << ": ";
		std::getline(std::cin, buffer);
		std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::tolower);

		auto params = Enqueue(buffer);
		command     = params.front();
		params.pop();

		Shell(command, params);

		if (command == "start") Start();
		if (command == "stop") Stop();
		if (command == "info") Info();
		if (command == "clear") system("cls");
		if (command == "exit") inContext = false;
	}
}