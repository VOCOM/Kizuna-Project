#include <error_handler.hpp>

#include <fstream>
#include <iostream>

#include <module.hpp>

ErrorQueue ErrorManager::errorQueue;

ErrorManager::ErrorManager() : running() {
	running = true;
	handler = std::thread(&ErrorManager::Loop, this);
}
ErrorManager::~ErrorManager() {
	running = false;
	if (handler.joinable()) handler.join();
}

void ErrorManager::Loop() const {
	std::ofstream fs("log.txt");
	while (running) {
		if (errorQueue.empty()) continue;

		Error& err = errorQueue.pop();
		if (HardFault* hf = dynamic_cast<HardFault*>(&err); hf != nullptr) {
			std::cout << hf->source << " faulted\n"
								<< hf->error << "\n"
								<< "Restarting...\n";
			Module::GetModule(err.source)->Restart();
		}

		fs << err.source << ": " << err.error << std::endl;
		errorQueue.pop();
	}
}
