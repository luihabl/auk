#include <SDL.h>

#include "tinysdl/platform/window.h"
#include "tinysdl/platform/log.h"
#include "tinysdl/graphics/graphics.h"

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

using namespace TinySDL;

namespace {
    SDL_Window * window = nullptr;
    SDL_GLContext context;
}

SDL_Window * Window::init(const char * name, int w, int h, int x, int y, uint32_t flags, bool use_vsync) {
    
    //Setting up logging so that it has colors on Windows
    Log::setup(Log::Level::Debug);
    
    //Initializing SDL
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    
    window = SDL_CreateWindow(name, x, y, w, h, flags);

    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    Log::info("SDL %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);

    //Initializing Open GL
    context = SDL_GL_CreateContext( window );
    SDL_GL_MakeCurrent(window, context);
    Graphics::load_functions(SDL_GL_GetProcAddress);
#ifdef DEBUG_OPENGL
    Graphics::setup_debug();
#endif
    SDL_GL_SetSwapInterval(use_vsync); //using VSync

    return window;
}

void Window::swap_buffers() {
    SDL_GL_SwapWindow(window);
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

void Window::get_drawable_size(int * w, int * h) {
    SDL_GL_GetDrawableSize(window, w, h);
}

SDL_GLContext * Window::get_context() {
    return &context;
}