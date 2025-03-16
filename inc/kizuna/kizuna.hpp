#ifndef KIZUNA
#define KIZUNA

#include <queue>
#include <string>
#include <thread>
#include <vector>

#include <error_handler.hpp>
#include <module.hpp>
#include <shell.hpp>

class Kizuna : public Shell {
public: // Kernel Interface
	virtual void Access();

public:
	void Initialize();
	void Shutdown();
	void LoadModule(const std::shared_ptr<Module>& module);

	~Kizuna();

private:
	void HelpCommand(std::queue<std::string>& params);
	void ConfigCommand(std::queue<std::string>& params);
	void ModuleCommand(std::queue<std::string>& params, void (Module::*function)());

private:
	ErrorManager errorHandler;
};

#endif /* KIZUNA */
