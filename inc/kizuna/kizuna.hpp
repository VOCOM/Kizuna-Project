#ifndef KIZUNA
#define KIZUNA

#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "errors.hpp"
#include "kernel.hpp"
#include "module.hpp"
#include "status.hpp"

class Kizuna : public Kernel {
public:
	void LoadModule(std::shared_ptr<Module>);

	void Initialize();
	void Shutdown();

	~Kizuna() { Shutdown(); }

public: // Kernel Interface
	virtual void Access();

private:
	void HelpCommand(std::queue<std::string>& params);
	void ConfigCommand(std::queue<std::string>& params);
	void ModuleCommand(std::queue<std::string>& params, void (Module::*function)());

public: // Error Handler
	static std::vector<std::shared_ptr<Module>> ModuleList;
	static std::queue<Error> ErrorQueue;

private:
	void ErrorHandler();

private:
	StatusCode errorHandlerStatus;
	std::thread errorHandler;
};

#endif /* KIZUNA */
