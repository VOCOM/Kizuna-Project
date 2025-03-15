#ifndef KERNEL
#define KERNEL

#include <queue>
#include <string>

#include <utility/utils.hpp>

class Kernel {
public:
	virtual void Access() = 0;

	virtual void Clear() final { system("cls"); }
};

#endif /* KERNEL */
