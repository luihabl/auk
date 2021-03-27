#pragma once 

#include <glad/glad.h>

namespace TinySDL {

    struct Color;

    namespace Graphics {
        void load_gl_functions();
        void setup_gl_debug();
        void APIENTRY gl_debug_callback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *user_param);
        
        void clear(Color c);
        void viewport(int w, int h);
        void viewport(int x, int y, int w, int h);
    }
}