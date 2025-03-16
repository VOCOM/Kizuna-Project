#include <error_handler.hpp>

void Emitter::AddToQueue(const Error& error) {
	while (ErrorManager::instance->queueLock) std::this_thread::yield();
	ErrorManager::instance->queueLock = true;
	ErrorManager::instance->errorQueue.push(error);
	ErrorManager::instance->queueLock = false;
}

void Emitter::Raise(std::string errorMessage) {
	AddToQueue(SoftFault(name, errorMessage.c_str()));
}

void Emitter::Assert(std::string errorMessage) {
	AddToQueue(HardFault(name, errorMessage.c_str()));
}
