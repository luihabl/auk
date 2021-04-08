#include <glad/glad.h>

#include "tinysdl/graphics/renderer.h"
#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/matrix/matrix.h"
#include "tinysdl/matrix/color.h"
#include "tinysdl/graphics/graphics.h"

using namespace TinySDL;

SpriteRenderer::SpriteRenderer(Shader & shader) {
    this->shader = shader;
    
    glGenVertexArrays(1, &this->quad_vao_id);
    glBindVertexArray(this->quad_vao_id);
    
    float vertices[] = {
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f,
        0.0f, 1.0f 
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    int indices[] = {0, 2, 1, 0, 3, 2};
    
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &uv_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quad_vao_id);
    glDeleteBuffers(1, &this->uv_vbo_id);
}

Mat4x4 SpriteRenderer::gen_model(const Vec2 & pos, const Vec2 & size, const float & rot) {
    Mat4x4 model = Mat4x4::identity();
    
    MatrixMath::translate(model, pos[0], pos[1], 0.0f); 
    MatrixMath::translate(model, 0.5f * size[0], 0.5f * size[1], 0.0f); 
    MatrixMath::rotate(model, rot);
    MatrixMath::translate(model, -0.5f * size[0], -0.5f * size[1], 0.0f); 
    MatrixMath::scale(model, size[0], size[1], 1.0f);
    
    return model;
}

void SpriteRenderer::gl_draw() {
    glBindVertexArray(this->quad_vao_id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::set_uv(const Vec4 & src_rect) {

    Vec<float, 8> uv = {
        src_rect[0],                src_rect[1],    
        src_rect[0] + src_rect[2],  src_rect[1], 
        src_rect[0] + src_rect[2],  src_rect[1] + src_rect[3],
        src_rect[0],                src_rect[1] + src_rect[3]
    };

    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uv.data), uv.data.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void SpriteRenderer::draw(const Texture & tex, const Vec2 & pos, const Vec2 & size, float rot, const Color & color) {
    
    shader.use();
    shader.set_mat4x4("model", gen_model(pos, size, rot));
    shader.set_vec3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    Vec4 src_rect = {0.0f, 0.0f, 1.0f, 1.0f}; //x, y, w, h
    set_uv(src_rect);
    
    gl_draw();
}


void SpriteRenderer::draw(const Texture & tex, const Vec2 & pos, float scale, float rot, const Color & color) {

    shader.use();
    shader.set_mat4x4("model", gen_model(pos, {scale * (float) tex.w, scale * (float) tex.h}, rot));
    shader.set_vec3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    Vec4 src_rect = {0.0f, 0.0f, 1.0f, 1.0f}; //x, y, w, h
    set_uv(src_rect);
    
    gl_draw();
}



void SpriteRenderer::draw_rect(const Texture & tex, const Vec2 & pos, Vec4 src_rect) {
    shader.use();

    float w = abs(src_rect[2] - src_rect[0]) * (float) tex.w;
    float h = abs(src_rect[3] - src_rect[1]) * (float) tex.h;

    shader.set_mat4x4("model", gen_model(pos, {w, h}, 0.0f));
    shader.set_vec3("spriteColor", {1.0f, 1.0f, 1.0f});

    glActiveTexture(GL_TEXTURE0);
    tex.bind();
    
    set_uv({src_rect[0], src_rect[1], w / (float) tex.w, h / (float) tex.h}); //x, y, w, h
    
    gl_draw();
}


