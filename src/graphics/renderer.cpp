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

Mat4x4 SpriteRenderer::gen_model(const Vec4 & dst_rect, const float & rot) {
    Mat4x4 model = Mat4x4::identity();
    
    MatrixMath::translate(model, dst_rect[0], dst_rect[1], 0.0f); 
    if(rot != 0.0f){
        MatrixMath::translate(model, 0.5f * dst_rect[2], 0.5f * dst_rect[3], 0.0f); 
        MatrixMath::rotate(model, rot);
        MatrixMath::translate(model, -0.5f * dst_rect[2], -0.5f * dst_rect[3], 0.0f); 
    }
    MatrixMath::scale(model, dst_rect[2], dst_rect[3], 1.0f);
    
    return model;
}

void SpriteRenderer::gl_draw_quad() {
    glBindVertexArray(this->quad_vao_id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::set_uv(const Vec4 & src_rect, const Texture & tex) {

    Vec<float, 8> uv = {
         src_rect[0] / (float) tex.w,                  src_rect[1] / (float) tex.h,    
        (src_rect[0] + src_rect[2]) / (float) tex.w,   src_rect[1] / (float) tex.h, 
        (src_rect[0] + src_rect[2]) / (float) tex.w,  (src_rect[1] + src_rect[3]) / (float) tex.h,
         src_rect[0] / (float) tex.w,                 (src_rect[1] + src_rect[3]) / (float) tex.h
    };

    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uv.data), uv.data.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void SpriteRenderer::draw(const Texture & tex, const Vec4 & src_rect, const Vec4 & dst_rect, float rot) {
    
    shader.use();
    shader.set_mat4x4("model", gen_model(dst_rect, rot));

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    set_uv(src_rect, tex); //x, y, w, h
    
    gl_draw_quad();
}

void SpriteRenderer::draw(const Texture & tex, const Vec2 & pos, float scale, float rot) {
    draw(tex, tex.full_rect, {pos[0], pos[1], scale * (float) tex.w, scale * (float) tex.h}, rot);
}

