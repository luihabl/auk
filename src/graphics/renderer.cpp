#include <glad/glad.h>

#include "tinysdl/graphics/renderer.h"
#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/matrix/matrix.h"
#include "tinysdl/matrix/color.h"
#include "tinysdl/graphics/graphics.h"

using namespace TinySDL;

SpriteBatch::SpriteBatch() {
    
    glGenVertexArrays(1, &vao_id);
    
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // ---- Adding vertex attributes here ----

    //Vec2 Vertex.pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    //Vec2 Vertex.uv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

    //Color Vertex.color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) offsetof(Vertex, color));

    //Vec<uint8_t, 3> Vertex.cmix
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) offsetof(Vertex, cmix));
    
    // --------

    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteBatch::~SpriteBatch() {
    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteBuffers(1, &ebo_id);
}

void SpriteBatch::set_texture(Texture * tex) {
    current_tex = tex;
}

void SpriteBatch::draw(const Vec4 & src_rect, const Vec4 & dst_rect, float rot, bool centered) {

    const float w  = src_rect[2];
    const float h  = src_rect[3];

    Vec2 origin = Vec2::zeros();
    if (centered) origin = {w/2.0f, h/2.0f};

    Mat4x4 transform = MatrixMath::gen_transform({dst_rect[0], dst_rect[1]}, {dst_rect[2] / w, dst_rect[3] / h}, {0.0f, 0.0f}, rot);

    push_tex_quad(w, h, (float) current_tex->w, (float) current_tex->h, src_rect, transform);
}

void SpriteBatch::draw(const Vec4 & src_rect, const Vec2 & pos, const Vec2 & scale, float rot, bool centered) {

    const float w  = src_rect[2];
    const float h  = src_rect[3];
    
    Vec2 origin = Vec2::zeros();
    if (centered) origin = {w/2.0f, h/2.0f};

    
    Mat4x4 transform = MatrixMath::gen_transform(pos, scale, origin, rot);

    push_tex_quad(w, h, (float) current_tex->w, (float) current_tex->h, src_rect, transform);
}

void SpriteBatch::draw_rect_fill(const Vec4 & rect, const Color & color) {
    
    const float w  = rect[2];
    const float h  = rect[3];
    
    Mat4x4 transform = MatrixMath::gen_transform({rect[0], rect[1]}, {1.0f, 1.0f}, {0.0f, 0.0f}, 0.0f);
        
    unsigned int n = (unsigned int) vertices.size();
    indices.insert(indices.end(), { n + 0, n + 2, n + 1, n + 0, n + 3,  n + 2 });

    push_vertex(0.0f, 0.0f, 0.0f, 0.0f, transform, color, {0, 0, 255});
    push_vertex(w, 0.0f, 0.0f, 0.0f, transform, color, {0, 0, 255});
    push_vertex(w, h, 0.0f, 0.0f, transform, color, {0, 0, 255});
    push_vertex(0.0f, h, 0.0f, 0.0f, transform, color, {0, 0, 255});
}

inline void SpriteBatch::push_tex_quad(float w, float h, float tex_w, float tex_h, const Vec4 & src_rect, const Mat4x4 & transform) {
    
    unsigned int n = (unsigned int) vertices.size();
    indices.insert(indices.end(), { n + 0, n + 2, n + 1, n + 0, n + 3,  n + 2 });

    push_vertex(0.0f, 0.0f, src_rect[0] / tex_w, src_rect[1] / tex_h, transform, Color::white, {255, 0, 0});
    push_vertex(w, 0.0f, (src_rect[0] + src_rect[2]) / tex_w,   src_rect[1] / tex_h, transform, Color::white, {255, 0, 0});
    push_vertex(w, h, (src_rect[0] + src_rect[2]) / tex_w,  (src_rect[1] + src_rect[3]) / tex_h, transform, Color::white, {255, 0, 0});
    push_vertex(0.0f, h, src_rect[0] / tex_w, (src_rect[1] + src_rect[3]) / tex_h, transform, Color::white, {255, 0, 0});
}

inline void SpriteBatch::push_vertex(const float & x, const float & y, const float & uv_x, const float & uv_y, const Mat4x4 & model, const Color & color, const ByteVec3 & cmix) {
    vertices.push_back({
        {model[0 + 0 * 4] * x +  model[0 + 1 * 4] * y + model[0 + 3 * 4],
         model[1 + 0 * 4] * x +  model[1 + 1 * 4] * y + model[1 + 3 * 4]},
        {uv_x, uv_y}, color, cmix
    });
}

void SpriteBatch::render() {

    glBindVertexArray(vao_id);

    // Binding texture
    glActiveTexture(GL_TEXTURE0);
    current_tex->bind();

    // Uploading vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    // Uploading indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

    //Draw everything
    glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, (void*) 0);

    glBindVertexArray(0);

    vertices.clear();
    indices.clear();
}