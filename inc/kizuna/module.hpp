#ifndef MODULE
#define MODULE

#include <queue>
#include <string>

#include <kizuna/kernel.hpp>
#include <kizuna/status.hpp>

class Module : public Kernel {
public:
	virtual std::string Name()       = 0;
	virtual std::string Status()     = 0;
	virtual void Info()              = 0;
	virtual void Start()             = 0;
	virtual void Stop()              = 0;
	virtual void Restart()           = 0;
	virtual void LoadConfiguration() = 0;

	virtual ~Module() {}
};

#endif /* MODULE */
