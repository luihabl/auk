#pragma once 
#include <SDL.h>

namespace TinySDL {
    namespace Window {
        SDL_Window * init(char * name, int w, int h, int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED, 
                          uint32_t flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, bool use_vsync = true);
        void close();
        SDL_Window * get_window();
        SDL_GLContext * get_context();
    }
}
