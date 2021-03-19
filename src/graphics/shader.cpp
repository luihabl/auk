#include <string>
#include <glad/glad.h>

#include "tinysdl/graphics/shader.h"
#include "tinysdl/core/log.h"

using namespace TinySDL;

Shader & Shader::use() {   
    glUseProgram(this->id);
    return *this;
}

unsigned int Shader::add(const char* src, GLenum type) {

    unsigned int id_shader = glCreateShader(type);
    GLint src_len = (GLint) strlen(src);

    glShaderSource(id_shader, 1, &src, &src_len);
    glCompileShader(id_shader);

    int success;
    char err_msg[1024];
    glGetShaderiv(id_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id_shader, 1024, NULL, err_msg);
        Log::error("Error compiling shader: %s\n", err_msg);
    }
 
    glAttachShader(id, id_shader);
    return id_shader;
}

void Shader::compile_all(const char* vertex_src, const char* frag_src, const char*  geom_src) {
    
    unsigned int id_vertex, id_frag, id_geom;

    this->id = glCreateProgram();

    id_vertex = add(vertex_src, GL_VERTEX_SHADER);
    id_frag   = add(frag_src, GL_FRAGMENT_SHADER);
    if (geom_src != nullptr) 
        id_geom  = add(geom_src, GL_GEOMETRY_SHADER);
    
    glLinkProgram(this->id);

    int success;
    char err_msg[1024];
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->id, 1024, NULL, err_msg);
        Log::error("Error linking shaders: %s\n", err_msg);
    }

    glDeleteShader(id_vertex);
    glDeleteShader(id_frag);
    if (geom_src != nullptr) 
        glDeleteShader(id_geom);

    this->compiled = (bool) success;
}

Shader Shader::from_source(const char* vertex_src, const char* frag_src, const char*  geom_src) {
    Shader shader;
    shader.compile_all(vertex_src, frag_src, geom_src);
    return shader;
}

Shader Shader::from_file(const char* vertex_path, const char* frag_path, const char*  geom_path = nullptr) {
    Shader shader;
    
}