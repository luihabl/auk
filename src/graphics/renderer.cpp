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

    Vec2 origin = Vec2::zeros;
    if (centered) origin = {w/2.0f, h/2.0f};

    Mat3x2 transform = MatrixMath::gen_transform_2d({dst_rect[0], dst_rect[1]}, {dst_rect[2] / w, dst_rect[3] / h}, {0.0f, 0.0f}, rot);

    push_quad(
        0, 0, w, 0, w, h, 0, h,
        src_rect[0] / (float) current_tex->w, src_rect[1] / (float) current_tex->h,
        (src_rect[0] + src_rect[2]) / (float) current_tex->w,   src_rect[1] / (float) current_tex->h,
        (src_rect[0] + src_rect[2]) / (float) current_tex->w,  (src_rect[1] + src_rect[3]) / (float) current_tex->h,
        src_rect[0] / (float) current_tex->w, (src_rect[1] + src_rect[3]) / (float) current_tex->h,
        Color::white,
        {255, 0, 0},
        transform
    );
}

void SpriteBatch::draw(const Vec4 & src_rect, const Vec2 & pos, const Vec2 & scale, float rot, bool centered) {

    const float w  = src_rect[2];
    const float h  = src_rect[3];
    
    Vec2 origin = Vec2::zeros;
    if (centered) origin = {w/2.0f, h/2.0f};

    
    Mat3x2 transform = MatrixMath::gen_transform_2d(pos, scale, origin, rot);

    push_quad(
        0, 0, w, 0, w, h, 0, h,
        src_rect[0] / (float) current_tex->w,                 src_rect[1] / (float) current_tex->h,
        (src_rect[0] + src_rect[2]) / (float) current_tex->w, src_rect[1] / (float) current_tex->h,
        (src_rect[0] + src_rect[2]) / (float) current_tex->w, (src_rect[1] + src_rect[3]) / (float) current_tex->h,
        src_rect[0] / (float) current_tex->w,                 (src_rect[1] + src_rect[3]) / (float) current_tex->h,
        Color::white,
        {255, 0, 0},
        transform
    );

}

void SpriteBatch::draw_rect_fill(const Vec4 & rect, const Color & color) {
    
    const float x = rect[0];
    const float y = rect[1];
    const float w = rect[2];
    const float h = rect[3];

    
    //Mat4x4 transform = MatrixMath::gen_transform({rect[0], rect[1]}, {1.0f, 1.0f}, {0.0f, 0.0f}, 0.0f);
    Mat3x2 transform = Mat3x2::identity;

    push_quad(
        x, y, x + w, y, x + w, y + h, x, y + h,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255},
        transform
    );
}

void SpriteBatch::draw_rect_line(const Vec4 & rect, const Color & color, const float & t) {
    
    const float x = rect[0];
    const float y = rect[1];
    const float w = rect[2];
    const float h = rect[3];

    //Mat4x4 transform = MatrixMath::gen_transform({x, y}, {1.0f, 1.0f}, {0.0f, 0.0f}, 0.0f);
    Mat3x2 transform = Mat3x2::identity;

    push_quad(
        x, y, 
        x + w - t, y, 
        x + w - t, y + t, 
        x, y + t,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255},
        transform
    ); 
    
    push_quad(
        x + w - t, y, 
        x + w, y, 
        x + w, y + h - t, 
        x + w - t, y + h - t,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255},
        transform
    ); 

    push_quad(
        x + t, y + h - t, 
        x + w, y + h - t, 
        x + w, y + h, 
        x + t, y + h,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255},
        transform
    ); 

    push_quad(
        x, y + t, 
        x + t, y + t, 
        x + t, y + h, 
        x, y + h,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255},
        transform
    ); 
}


inline void SpriteBatch::push_quad(const float & x0, const float & y0, const float & x1, const float & y1, 
                                   const float & x2, const float & y2, const float & x3, const float & y3, 
                                   const float & uv_x0, const float & uv_y0, const float & uv_x1, const float & uv_y1, 
                                   const float & uv_x2, const float & uv_y2, const float & uv_x3, const float & uv_y3, 
                                   const Color & color, const ByteVec3 & cmix, const Mat3x2 & model) {

    const unsigned int n = (unsigned int) vertices.size();
    indices.insert(indices.end(), { n + 0, n + 2, n + 1, n + 0, n + 3,  n + 2 });

    push_vertex(x0, y0, uv_x0, uv_y0, model, color, cmix);
    push_vertex(x1, y1, uv_x1, uv_y1, model, color, cmix);
    push_vertex(x2, y2, uv_x2, uv_y2, model, color, cmix);
    push_vertex(x3, y3, uv_x3, uv_y3, model, color, cmix);
}

inline void SpriteBatch::push_vertex(const float & x, const float & y, const float & uv_x, const float & uv_y, 
                                     const Mat3x2 & model, const Color & color, const ByteVec3 & cmix) {
    vertices.push_back({
        {model[0] * x +  model[2] * y + model[4],
         model[1] * x +  model[3] * y + model[5]},
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