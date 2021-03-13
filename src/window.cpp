#include <window.h>

using namespace TinySDL;

namespace {
    SDL_Window * window = nullptr;
}

void Window::init(std::string name, int w, int h ) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE);
}

void Window::close() {
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

SDL_Window * Window::get() {
    return window;
}