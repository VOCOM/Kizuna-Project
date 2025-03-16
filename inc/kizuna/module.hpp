#ifndef MODULE
#define MODULE

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <shell.hpp>

class Module : public Shell {
public:
	enum StatusCode {
		Offline,
		Online,
		Busy,
		Faulted
	};
	std::string ToString(StatusCode& status) {
		return status == Online    ? "Online"
		       : status == Busy    ? "Busy"
		       : status == Faulted ? "Faulted"
		                           : "Offline";
	}

public:
	virtual std::string Name()       = 0;
	virtual std::string Status()     = 0;
	virtual void Info()              = 0;
	virtual void Start()             = 0;
	virtual void Stop()              = 0;
	virtual void Restart()           = 0;
	virtual void LoadConfiguration() = 0;

	static bool IsOnline(std::string moduleName);
	static std::shared_ptr<Module> GetModule(std::string moduleName);
	static std::vector<std::shared_ptr<Module>>& GetModules();
	static void Shutdown();

	virtual ~Module() {}

private:
	static std::vector<std::shared_ptr<Module>> modules;
};

#endif /* MODULE */
