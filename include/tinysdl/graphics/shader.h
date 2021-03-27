#pragma once 

#include <glad/glad.h>
#include "tinysdl/matrix/matrix.h"

namespace TinySDL {
    class Shader {
        public:
            Shader() = default;
            
            unsigned int id;
            bool compiled = false;
            
            Shader & use();
            void del();
            
            void set_float(const char * name, const float & value) const;
            void set_int(const char * name, const int & value) const;
            void set_mat4x4(const char * name, const Mat4x4 & mat) const;
            void set_vec3(const char * name, const Vec3 & vec) const;
            // add other uniform types

            static Shader from_source(const char* vertex_src, const char* frag_src, const char*  geom_src = nullptr);
            static Shader from_file(const char* vertex_path, const char* frag_path, const char*  geom_path = nullptr);
            static Shader default_sprite_shaders();

        private: 
            unsigned int add(const char* source, GLenum type);
            void compile_all(const char* vertex_src, const char* frag_src, const char*  geom_src = nullptr);
    };
}