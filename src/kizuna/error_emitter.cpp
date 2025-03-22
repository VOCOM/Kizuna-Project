#include <error_handler.hpp>

void Emitter::AddToQueue(const Error error) {
	ErrorManager::errorQueue.push(error);
}

void Emitter::Raise(std::string errorMessage) {
	AddToQueue(SoftFault(name, errorMessage.c_str()));
}

void Emitter::Assert(std::string errorMessage) {
	AddToQueue(HardFault(name, errorMessage.c_str()));
}
