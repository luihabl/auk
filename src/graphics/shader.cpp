#include "auk/graphics/shader.h"

#include <glad/glad.h>

#include <cstring>
#include <string>

#include "auk/numerics/matrix.h"
#include "auk/platform/file.h"
#include "auk/platform/log.h"

constexpr const char* default_vertex_src =
    "#version 330 core\n\
layout (location = 0) in vec2 vertex_pos;\n\
layout (location = 1) in vec2 vertex_uv;\n\
layout (location = 2) in vec4 vertex_color;\n\
layout (location = 3) in vec3 vertex_cmix;\n\
out vec2 uv;\n\
out vec4 color;\n\
out vec3 cmix;\n\
uniform mat4 projection;\n\
void main()\n\
{\n\
    uv = vertex_uv;\n\
    color = vertex_color;\n\
    cmix = vertex_cmix;\n\
    gl_Position = projection * vec4(vertex_pos.xy, 0.0, 1.0);\n\
}";

constexpr const char* default_frag_src =
    "#version 330 core\n\
in vec2 uv;\n\
in vec4 color;\n\
in vec3 cmix;\n\
out vec4 output_color;\n\
uniform sampler2D image;\n\
void main()\n\
{\n\
    vec4 tex_color = texture(image, uv);\n\
    output_color = tex_color * color * cmix.x + tex_color.a * color * cmix.y + color * cmix.z;\n\
}";

using namespace auk;

Shader& Shader::use() {
    if (this->compiled) {
        glUseProgram(this->id);
    } else {
        Log::warn("Trying to use empty shader.");
    }

    return *this;
}

void Shader::del() {
    glDeleteProgram(this->id);
    compiled = false;
}

unsigned int Shader::add(const char* src, unsigned int type) {
    unsigned int id_shader = glCreateShader((GLenum)type);
    GLint src_len = (GLint)strlen(src);

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

void Shader::compile_all(const char* vertex_src, const char* frag_src, const char* geom_src) {
    unsigned int id_vertex, id_frag, id_geom = 0;

    this->id = glCreateProgram();

    id_vertex = add(vertex_src, GL_VERTEX_SHADER);
    id_frag = add(frag_src, GL_FRAGMENT_SHADER);
    if (geom_src != nullptr)
        id_geom = add(geom_src, GL_GEOMETRY_SHADER);

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

    this->compiled = (bool)success;
}

Shader Shader::from_source(const char* vertex_src, const char* frag_src, const char* geom_src) {
    Shader shader;
    shader.compile_all(vertex_src, frag_src, geom_src);
    return shader;
}

Shader Shader::from_file(const char* vertex_path, const char* frag_path, const char* geom_path) {
    Shader shader;
    std::string vertex_src, frag_src, geom_src;

    vertex_src = File::load_txt(vertex_path);
    frag_src = File::load_txt(frag_path);
    if (geom_path != nullptr)
        geom_src = File::load_txt(geom_path);

    if (frag_src != "" && vertex_src != "")
        shader.compile_all(vertex_src.c_str(), frag_src.c_str(),
                           geom_path != nullptr ? geom_src.c_str() : nullptr);
    return shader;
}

Shader Shader::default_sprite_shaders() {
    return from_source(default_vertex_src, default_frag_src);
}

void Shader::set_float(const char* name, const float& value) const {
    glUniform1f(glGetUniformLocation(this->id, name), value);
}

void Shader::set_double(const char* name, const double& value) const {
    glUniform1d(glGetUniformLocation(this->id, name), value);
}

void Shader::set_int(const char* name, const int& value) const {
    glUniform1i(glGetUniformLocation(this->id, name), value);
}

void Shader::set_mat4x4(const char* name, const Mat4x4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, mat.data);
}

void Shader::set_vec2(const char* name, const Vec2& vec) const {
    glUniform2f(glGetUniformLocation(this->id, name), vec[0], vec[1]);
}

void Shader::set_vec3(const char* name, const Vec3& vec) const {
    glUniform3f(glGetUniformLocation(this->id, name), vec[0], vec[1], vec[2]);
}

void Shader::set_dvec2(const char* name, const DVec2& vec) const {
    glUniform2d(glGetUniformLocation(this->id, name), vec[0], vec[1]);
}