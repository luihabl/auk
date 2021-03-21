#include <string>
#include <glad/glad.h>

#include "tinysdl/graphics/shader.h"
#include "tinysdl/core/log.h"
#include "tinysdl/core/file.h"
#include "tinysdl/math/matrix.h"

constexpr char* default_vertex_src = "#version 460\n\
in mediump vec3 point;\n\
in mediump vec2 texcoord;\n\
out mediump vec2 UV;\n\
void main()\n\
{\n\
  gl_Position = vec4(point, 1);\n\
  UV = texcoord;\n\
}";

constexpr char* default_frag_src = "#version 460\n\
in mediump vec2 UV;\n\
out mediump vec3 fragColor;\n\
uniform sampler2D tex;\n\
void main()\n\
{\n\
  fragColor = texture(tex, UV).rgb;\n\
}";

using namespace TinySDL;

Shader & Shader::use() {
    if(this->compiled) {
        glUseProgram(this->id);
    }        
    else {
        Log::warn("Trying to use empty shader.");
    }
        
    return *this;
}

void Shader::del() {
    glDeleteProgram(this->id);
    compiled = false;
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

Shader Shader::from_file(const char* vertex_path, const char* frag_path, const char*  geom_path) {
    Shader shader;
    std::string vertex_src, frag_src, geom_src;

    vertex_src = File::read(vertex_path);
    frag_src   = File::read(frag_path);
    if(geom_path != nullptr)
        geom_src   = File::read(geom_path);

    if (frag_src != "" && vertex_src != "")
        shader.compile_all(vertex_src.c_str(), frag_src.c_str(), geom_path != nullptr ? geom_src.c_str() : nullptr);
    return shader;    
}

Shader Shader::default_sprite_shaders() {
    return from_source(default_vertex_src, default_frag_src);
}


void Shader::set_int(const char * name, int value) {
    glUniform1i(glGetUniformLocation(this->id, name), value);
}


void Shader::set_mat4x4(const char * name, Mat4x4 & mat){
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, mat.data);
}

void Shader::set_vec3(const char * name, Vec3 & vec){
    glUniform3f(glGetUniformLocation(this->id, name), vec[0], vec[1], vec[2]);
}