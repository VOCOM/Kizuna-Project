#ifndef SHELL
#define SHELL

#include <queue>
#include <string>

class Shell {
public:
	virtual void Access() = 0;

	virtual void Clear() final { system("cls"); }
};

#endif /* SHELL */
