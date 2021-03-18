#include <window.h>
#include <log.h>
#include <graphics/graphics.h>
#include <SDL.h>



using namespace TinySDL;

namespace {
    SDL_Window * window = nullptr;
    SDL_GLContext context;
}

void Window::init(char * name, int w, int h ) {
    
    Log::set_level(Log::Level::Info);
    
    //Initializing SDL
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext( window );

    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    Log::info("SDL %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);

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