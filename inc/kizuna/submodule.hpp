#ifndef SUBMODULE
#define SUBMODULE

class Submodule {
public:
	virtual void Info()              = 0;
	virtual void Start()             = 0;
	virtual void Stop()              = 0;
	virtual void Restart()           = 0;
	virtual void LoadConfiguration() = 0;

	Submodule() = delete;
	Submodule(const char* name) : name(name) {}
	virtual ~Submodule() {}

public:
	std::string Name() const { return name; }
	std::string Status() const;

protected:
	const char* name  = nullptr;
	StatusCode status = StatusCode::Offline;
};

#endif /* SUBMODULE */
