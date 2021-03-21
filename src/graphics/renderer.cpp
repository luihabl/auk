#include <glad/glad.h>

#include "tinysdl/graphics/renderer.h"
#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/math/matrix.h"

using namespace TinySDL;

SpriteRenderer::SpriteRenderer(Shader & shader) {
    this->shader = shader;
    this->init();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quad_vao);
}

void SpriteRenderer::init() {

    unsigned int vbo;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quad_vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quad_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


void SpriteRenderer::draw(SpriteTexture & tex, Vec2 pos, Vec2 size, float rot, Vec3 color) {
    shader.use();

    Mat4x4 model = Mat4x4::identity();
    MatrixMath::translate(model, 0.5f * size[0], 0.5f * size[1], 0.0f); 
    MatrixMath::rotate(model, rot);
    MatrixMath::translate(model, -0.5f * size[0], -0.5f * size[1], 0.0f); 
    MatrixMath::translate(model, size[0], size[1], 1.0f);

    shader.set_mat4x4("model", model);
    shader.set_vec3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    glBindVertexArray(this->quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
