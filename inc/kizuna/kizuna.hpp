#ifndef KIZUNA
#define KIZUNA

#include <exception>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <thread>
#include <utility>

#include "status.hpp"
#include "submodule.hpp"

class Kizuna {
public:
	static void ErrorHandler();
	static void StartErrorHandler();
	static void StopErrorHandler();

	static void LoadSubmodule(std::shared_ptr<Submodule>);
	static void Shutdown();

	static std::vector<std::shared_ptr<Submodule>> SubmoduleList;
	static std::queue<std::exception> ErrorQueue;

private:
	static StatusCode errorHandlerStatus;
	static std::thread errorHandler;
};

#endif /* KIZUNA */
