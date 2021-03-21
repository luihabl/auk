#include <SDL.h>

#include "tinysdl/core/window.h"
#include "tinysdl/core/log.h"
#include "tinysdl/graphics/graphics.h"

#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 6

using namespace TinySDL;

namespace {
    SDL_Window * window = nullptr;
    SDL_GLContext context;
}

SDL_Window * Window::init(char * name, int w, int h, int x, int y, uint32_t flags) {
    
    //Setting up logging so that it has colors on Windows
    Log::setup();
    
    //Initializing SDL
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    window = SDL_CreateWindow(name, x, y, w, h, flags);

    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    Log::info("SDL %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);

    //Initializing Open GL
    context = SDL_GL_CreateContext( window );
    Graphics::load_gl_functions();

    return window;
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