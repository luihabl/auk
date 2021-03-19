#include <SDL.h>

#include "tinysdl/core/window.h"
#include "tinysdl/core/log.h"
#include "tinysdl/graphics/graphics.h"

using namespace TinySDL;

namespace {
    SDL_Window * window = nullptr;
    SDL_GLContext context;
}

void Window::init(char * name, int w, int h ) {
    
    Log::setup();
    
    //Initializing SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    Log::info("SDL %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);

    context = SDL_GL_CreateContext( window );
    Graphics::init();
}

void Window::close() {
	SDL_DestroyWindow(window);
	window = nullptr;
    SDL_GL_DeleteContext(context);
	SDL_Quit();
}

SDL_Window * Window::get_window() {
    return window;
}

SDL_GLContext * Window::get_context() {
    return &context;
}