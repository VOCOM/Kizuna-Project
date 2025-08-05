#include <kizuna.hpp>

Kizuna::Kizuna() {
	window = SDL_CreateWindow("Hello World", 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window) {
		SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
		return;
	}
	glContext = SDL_GL_CreateContext(window);
}
Kizuna::~Kizuna() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Kizuna::Run() {
	running = true;

	while (running) {
		EventLoop();

		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
		SDL_Delay(16);
	}
}

void Kizuna::Quit() {
	SDL_Event quitEvent;
	SDL_zero(quitEvent);
	quitEvent.type = SDL_EVENT_QUIT;
	SDL_PushEvent(&quitEvent);
}

void Kizuna::EventLoop() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT:
			running = false;
			break;

		default:
			break;
		}
	}
}
