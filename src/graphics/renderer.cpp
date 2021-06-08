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
    shader.set_mat4x4("model", MatrixMath::gen_model(dst_rect, rot));

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    set_uv(src_rect, tex); //x, y, w, h
    
    gl_draw_quad();
}

void SpriteRenderer::draw(const Texture & tex, const Vec2 & pos, float scale, float rot) {
    draw(tex, tex.full_rect, {pos[0], pos[1], scale * (float) tex.w, scale * (float) tex.h}, rot);
}



// ------------ SpriteBatch ------------

SpriteBatch::SpriteBatch() {
    
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);


    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void SpriteBatch::draw(const Texture & tex, const Vec4 & src_rect, const Vec4 & dst_rect, float rot) {

    Mat4x4 model = MatrixMath::gen_model(dst_rect, rot);

    // int n = (int) vertices.size()
    // change to indices.insert(indices.end(), { n + 0, n + 2, n + 1, n + 0, n + 3,  n + 2 });

    indices.push_back((unsigned int) vertices.size() + 0);
    indices.push_back((unsigned int) vertices.size() + 2);
    indices.push_back((unsigned int) vertices.size() + 1);
    indices.push_back((unsigned int) vertices.size() + 0);
    indices.push_back((unsigned int) vertices.size() + 3);
    indices.push_back((unsigned int) vertices.size() + 2);

    Vertex v0;
    v0.pos[0] = model.data[0 + 0 * 4] * 0.0f +  model.data[0 + 1 * 4] * 0.0f + model.data[0 + 3 * 4];
    v0.pos[1] = model.data[1 + 0 * 4] * 0.0f +  model.data[1 + 1 * 4] * 0.0f + model.data[1 + 3 * 4];
    v0.uv = {src_rect[0] / (float) tex.w, src_rect[1] / (float) tex.h};
    vertices.push_back(v0);

    Vertex v1;
    v1.pos[0] = model.data[0 + 0 * 4] * 1.0f +  model.data[0 + 1 * 4] * 0.0f + model.data[0 + 3 * 4];
    v1.pos[1] = model.data[1 + 0 * 4] * 1.0f +  model.data[1 + 1 * 4] * 0.0f + model.data[1 + 3 * 4];
    v1.uv = {(src_rect[0] + src_rect[2]) / (float) tex.w,   src_rect[1] / (float) tex.h};
    vertices.push_back(v1);

    Vertex v2;
    v2.pos[0] = model.data[0 + 0 * 4] * 1.0f +  model.data[0 + 1 * 4] * 1.0f + model.data[0 + 3 * 4];
    v2.pos[1] = model.data[1 + 0 * 4] * 1.0f +  model.data[1 + 1 * 4] * 1.0f + model.data[1 + 3 * 4];
    v2.uv = {(src_rect[0] + src_rect[2]) / (float) tex.w,  (src_rect[1] + src_rect[3]) / (float) tex.h};
    vertices.push_back(v2);

    Vertex v3;
    v3.pos[0] = model.data[0 + 0 * 4] * 0.0f +  model.data[0 + 1 * 4] * 1.0f + model.data[0 + 3 * 4];
    v3.pos[1] = model.data[1 + 0 * 4] * 0.0f +  model.data[1 + 1 * 4] * 1.0f + model.data[1 + 3 * 4];
    v3.uv = {src_rect[0] / (float) tex.w, (src_rect[1] + src_rect[3]) / (float) tex.h};
    vertices.push_back(v3);
}

void SpriteBatch::push_vertex(const Vertex & vertex) {
    // Do other stuff here

    
}

void SpriteBatch::render(Shader & shader, Texture & tex) {

    // maybe can remove this afterwards vvvvvvv    
    glActiveTexture(GL_TEXTURE0);
    tex.bind();
    // ^^^^^^^^^^

    glBindVertexArray(vao_id);

    // Uploading vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    // Uploading indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

    //Draw everything
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*) 0);

    glBindVertexArray(0);

    vertices.clear();
    indices.clear();
}