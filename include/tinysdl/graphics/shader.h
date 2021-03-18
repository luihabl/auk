#pragma once 

#include <glad/glad.h>

namespace TinySDL {
    class Shader {
        public:
            
            Shader() = default;

            unsigned int id;
            
            Shader & use();
            
            void set_float(const char * name, float value);
            void set_int(const char * name, int value);
            // add other uniform types

        private: 
            void init();
            void add_shader(const char* source, GLenum type);
    };
}