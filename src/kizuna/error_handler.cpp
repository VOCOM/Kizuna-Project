#include <error_handler.hpp>

#include <iostream>

#include <module.hpp>

std::queue<Error> ErrorManager::errorQueue;

void ErrorManager::Start() {
	running = true;
	handler = std::thread(&ErrorManager::Loop, this);
}
void ErrorManager::Stop() {
	running = false;
	if (handler.joinable()) handler.join();
}

void ErrorManager::Queue(const Error& e) {
	errorQueue.push(e);
}

void ErrorManager::Loop() {
	while (running) {
		if (errorQueue.empty()) {
			std::this_thread::yield();
			continue;
		}

		Error& err = errorQueue.front();
		if (HardFault* hf = dynamic_cast<HardFault*>(&err); hf != nullptr) {
			std::cout << hf->source << " faulted\n"
								<< hf->error << "\n"
								<< "Restarting...\n";
			Module::GetModule(err.source)->Restart();
		}

		errorQueue.pop();
	}
}
