#pragma once 
#include <SDL.h>

namespace TinySDL {
    namespace Window {
        SDL_Window * init(const char * name, int w, int h, int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED, 
                          uint32_t flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, bool use_vsync = true);
        void close();
        void swap_buffers();
        void get_drawable_size(int * w, int * h);
        SDL_Window * get_window();
        SDL_GLContext * get_context();
    }
}
