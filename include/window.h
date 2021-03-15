#pragma once 
#include <SDL.h>

namespace TinySDL {
    namespace Window {
        void init(char * name, int w, int h);
        void close();
        SDL_Window * get_window();
        SDL_GLContext * get_context();
    }
}
