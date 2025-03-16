#include <error_handler.hpp>

#include <iostream>

#include <module.hpp>

std::shared_ptr<ErrorManager> ErrorManager::instance;

void ErrorManager::Start() {
	running = true;
	handler = std::thread(&ErrorManager::Loop, this);
}
void ErrorManager::Stop() {
	running = false;
	if (handler.joinable()) handler.join();
}

void ErrorManager::Loop() {
	while (running) {
		if (errorQueue.empty()) {
			queueLock = false;
			std::this_thread::yield();
			continue;
		}

		queueLock  = true;
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
