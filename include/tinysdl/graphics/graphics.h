#pragma once 

typedef void* (* GraphicsLoaderFunction)(const char *name);

namespace TinySDL {

    struct Color;

    namespace Graphics {
        void load_functions(GraphicsLoaderFunction loader_func);
        void setup_debug();
        
        void clear(const Color & c);
        void viewport(int w, int h);
        void viewport(int x, int y, int w, int h);
    }
}