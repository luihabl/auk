#pragma once 

#include <glad/glad.h>
#include "tinysdl/math/matrix.h"

namespace TinySDL {
    class Shader {
        public:
            Shader() = default;
            
            unsigned int id;
            bool compiled = false;
            
            Shader & use();
            void del();
            
            void set_float(const char * name, float value);
            void set_int(const char * name, int value);
            void set_mat4x4(const char * name, Mat4x4 & mat);
            void set_vec3(const char * name, Vec3 & vec);
            // add other uniform types

            static Shader from_source(const char* vertex_src, const char* frag_src, const char*  geom_src = nullptr);
            static Shader from_file(const char* vertex_path, const char* frag_path, const char*  geom_path = nullptr);
            static Shader default_sprite_shaders();

        private: 
            unsigned int add(const char* source, GLenum type);
            void compile_all(const char* vertex_src, const char* frag_src, const char*  geom_src = nullptr);
    };
}