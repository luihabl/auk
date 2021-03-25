#pragma once 

#include <glad/glad.h>
#include "tinysdl/math/matrix.h"

namespace TinySDL {
    namespace Graphics {
        void load_gl_functions();
        void setup_gl_debug();
        void APIENTRY gl_debug_callback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *user_param);
        
        void clear(Vec4 color);
        void viewport(int w, int h);
        void viewport(int x, int y, int w, int h);
    }
}