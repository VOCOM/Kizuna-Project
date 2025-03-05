#ifndef KIZUNA
#define KIZUNA

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <CL/opencl.hpp>

class Configuration {
public:
	static void ListConfig(std::string filter = "");
	static void LoadConfig();

	static std::map<std::string, std::map<std::string, std::string>> Config;

	Configuration() = delete;
};

class Submodule {
public:
	virtual void Info()              = 0;
	virtual void Start()             = 0;
	virtual void Stop()              = 0;
	virtual void Restart()           = 0;
	virtual void LoadConfiguration() = 0;

	virtual ~Submodule() {}

public:
	const char* Name = nullptr;
};

class Kizuna {
public:
	static void LoadSubmodule(std::shared_ptr<Submodule>);
	static void Shutdown();

	static std::vector<std::shared_ptr<Submodule>> submodules;
};

#endif /* KIZUNA */
