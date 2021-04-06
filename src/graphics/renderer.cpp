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
    this->init();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quad_vao);
}

void SpriteRenderer::init() {

    glGenVertexArrays(1, &this->quad_vao);
    glBindVertexArray(this->quad_vao);
    
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

    int indices[] = {
        0, 2, 1,
        0, 3, 2
    };
    
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    float tex_vertices[] = {
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f,
        0.0f, 1.0f 
    };

    GLuint tex_buffer;
    glGenBuffers(1, &tex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_vertices), tex_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::draw(const Texture & tex, const Vec2 & pos, const Vec2 & size, float rot, const Color & color) {
    shader.use();

    Mat4x4 model = Mat4x4::identity();
    
    MatrixMath::translate(model, pos[0], pos[1], 0.0f); 
    MatrixMath::translate(model, 0.5f * size[0], 0.5f * size[1], 0.0f); 
    MatrixMath::rotate(model, rot);
    MatrixMath::translate(model, -0.5f * size[0], -0.5f * size[1], 0.0f); 
    MatrixMath::scale(model, size[0], size[1], 1.0f);

    shader.set_mat4x4("model", model);
    shader.set_vec3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    glBindVertexArray(this->quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::draw(const Texture & tex, const Vec2 & pos, float scale, float rot, const Color & color) {
    shader.use();

    Mat4x4 model = Mat4x4::identity();
    Vec2 size = {scale * (float) tex.w, scale * (float) tex.h};
    
    MatrixMath::translate(model, pos[0], pos[1], 0.0f); 
    MatrixMath::translate(model, 0.5f * size[0], 0.5f * size[1], 0.0f); 
    MatrixMath::rotate(model, rot);
    MatrixMath::translate(model, -0.5f * size[0], -0.5f * size[1], 0.0f); 
    MatrixMath::scale(model, size[0], size[1], 1.0f);

    shader.set_mat4x4("model", model);
    shader.set_vec3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    glBindVertexArray(this->quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

