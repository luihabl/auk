#include <string>
#include <glad/glad.h>

#include "tinysdl/graphics/shader.h"
#include "tinysdl/core/log.h"

using namespace TinySDL;

void Shader::init() {
    this->id = glCreateProgram();
}

Shader & Shader::use() {
    glUseProgram(this->id);
    return *this;
}

void Shader::add_shader(const char* source, GLenum type) {

    unsigned int id_shader = glCreateShader(type);
    GLint code_length = strlen(source);

    glShaderSource(id_shader, 1, &source, &code_length);
    glCompileShader(id_shader);

    int success;
    char err_msg[512];
    glGetShaderiv(id_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id_shader, 512, nullptr, err_msg);
        Log::error("Error compiling shader: %s\n", err_msg);
    }
 
    glAttachShader(id, id_shader);
}
