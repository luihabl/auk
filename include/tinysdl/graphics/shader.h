#pragma once 

#include <glad/glad.h>

namespace TinySDL {
    class Shader {
        public:
            unsigned int id;
            bool compiled = false;
            
            Shader & use();
            
            void set_float(const char * name, float value);
            void set_int(const char * name, int value);
            // add other uniform types

            static Shader from_source(const char* vertex_src, const char* frag_src, const char*  geom_src = nullptr);
            static Shader from_file(const char* vertex_path, const char* frag_path, const char*  geom_path = nullptr);
            static Shader default_sprite_shaders();

        private: 
            Shader() = default;
            unsigned int add(const char* source, GLenum type);
            void compile_all(const char* vertex_src, const char* frag_src, const char*  geom_src = nullptr);
    };
}