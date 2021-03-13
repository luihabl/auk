#pragma once 
#include <string>
#include <SDL.h>

namespace TinySDL {
    namespace Window {
        void init(std::string name, int w, int h);
        void close();
        SDL_Window * get();
    }
}
