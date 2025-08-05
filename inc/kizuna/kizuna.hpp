#ifndef KIZUNA
#define KIZUNA

#include <thread>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

class Kizuna {
public:
	Kizuna();
	~Kizuna();

	void Run();
	void Quit();

private:
	void EventLoop();

private:
	bool running       = false;
	SDL_Window* window = nullptr;
	SDL_GLContext glContext;
};

#endif /* KIZUNA */
