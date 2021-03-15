#include <window.h>
#include <log.h>
#include <string>

using namespace TinySDL;

namespace {
    SDL_Window * window = nullptr;
}

void Window::init(std::string name, int w, int h ) {
    Log::set_level(Log::Level::Info);
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE);
    
    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    Log::info("SDL version %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);   
}

void Window::close() {
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

SDL_Window * Window::get() {
    return window;
}