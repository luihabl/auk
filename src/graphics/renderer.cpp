#include <glad/glad.h>
#include <cmath>

#include "tinysdl/graphics/renderer.h"
#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/graphics/color.h"

#include "tinysdl/numerics/matrix.h"
#include "tinysdl/numerics/rect.h"
#include "tinysdl/numerics/linalg.h"
#include "tinysdl/numerics/scalar.h"


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

    // ---- Initializing variables ----
    transform = Mat3x2::identity;
}

SpriteBatch::~SpriteBatch() {
    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteBuffers(1, &ebo_id);
}

void SpriteBatch::set_texture(Texture * tex) {
    current_tex = tex;
}

void SpriteBatch::push_transform(const Mat3x2 & new_transform) {
    transform_stack.push_back(transform);
    transform = LinAlg2D::matmul(new_transform, transform);
}

Mat3x2 SpriteBatch::pop_transform() {
    const Mat3x2 last_transform = transform;
    transform = transform_stack.back();
    transform_stack.pop_back();
    return last_transform;
}

void SpriteBatch::draw(const Rect & src, const Rect & dst, float rot, bool centered) {

    const float w  = src.w;
    const float h  = src.h;

    Vec2 origin = Vec2::zeros;
    if (centered) origin = {w/2.0f, h/2.0f};

    push_transform(LinAlg2D::gen_transform({dst.x, dst.y}, {dst.w / w, dst.h / h}, {0.0f, 0.0f}, rot));

    push_quad(
        0, 0, w, 0, w, h, 0, h,
        src.x / (float) current_tex->w, src.y / (float) current_tex->h,
        (src.x + src.w) / (float) current_tex->w, src.y / (float) current_tex->h,
        (src.x + src.w) / (float) current_tex->w, (src.y + src.h) / (float) current_tex->h,
        src.x / (float) current_tex->w, (src.y + src.h) / (float) current_tex->h,
        Color::white,
        {255, 0, 0}
    );

    pop_transform();
}


void SpriteBatch::draw(const Rect & src, const Vec2 & pos) {

    const float x = pos[0];
    const float y = pos[1];
    const float w  = src.w;
    const float h  = src.h;

    push_quad(
        x, y, x + w, y, x + w, y + h, x, y + h,
        src.x / (float) current_tex->w, src.y / (float) current_tex->h,
        (src.x + src.w) / (float) current_tex->w, src.y / (float) current_tex->h,
        (src.x + src.w) / (float) current_tex->w, (src.y + src.h) / (float) current_tex->h,
        src.x / (float) current_tex->w, (src.y + src.h) / (float) current_tex->h,
        Color::white,
        {255, 0, 0}
    );

}

void SpriteBatch::draw(const Rect & src, const Vec2 & pos, const Vec2 & scale, float rot, bool centered) {

    const float w  = src.w;
    const float h  = src.h;
    
    Vec2 origin = Vec2::zeros;
    if (centered) origin = {w/2.0f, h/2.0f};

    push_transform(LinAlg2D::gen_transform(pos, scale, origin, rot));

    push_quad(
        0, 0, w, 0, w, h, 0, h,
        src.x / (float) current_tex->w, src.y / (float) current_tex->h,
        (src.x + src.w) / (float) current_tex->w, src.y / (float) current_tex->h,
        (src.x + src.w) / (float) current_tex->w, (src.y + src.h) / (float) current_tex->h,
        src.x / (float) current_tex->w, (src.y + src.h) / (float) current_tex->h,
        Color::white,
        {255, 0, 0}
    );

    pop_transform();
}

void SpriteBatch::draw_rect_fill(const Rect & rect, const Color & color) {
    
    const float x = rect.x;
    const float y = rect.y;
    const float w = rect.w;
    const float h = rect.h;

    push_quad(
        x, y, x + w, y, x + w, y + h, x, y + h,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    );
}

void SpriteBatch::draw_rect_line(const Rect & rect, const Color & color, float t) {
    
    const float x = rect.x;
    const float y = rect.y;
    const float w = rect.w;
    const float h = rect.h;

    if (t >= 0.5f * h || t >= 0.5f * w) {
        draw_rect_fill(rect, color);
        return;
    }

    push_quad(
        x, y, 
        x + w - t, y, 
        x + w - t, y + t, 
        x, y + t,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    ); 
    
    push_quad(
        x + w - t, y, 
        x + w, y, 
        x + w, y + h - t, 
        x + w - t, y + h - t,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    ); 

    push_quad(
        x + t, y + h - t, 
        x + w, y + h - t, 
        x + w, y + h, 
        x + t, y + h,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    ); 

    push_quad(
        x, y + t, 
        x + t, y + t, 
        x + t, y + h, 
        x, y + h,
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    ); 
}

void SpriteBatch::draw_triangle_fill(const Vec2 & p0, const Vec2 & p1, const Vec2 & p2, const Color & color) {

    push_triangle(
        p0[0], p0[1], p1[0], p1[1], p2[0], p2[1],
        0, 0, 0, 0, 0, 0,
        color, 
        {0, 0, 255}
    );
}

void SpriteBatch::draw_triangle_line(const Vec2 & p0, const Vec2 & p1, const Vec2 & p2, float t, const Color & color) {

    Vec2 v01 = (p1 - p0).normalized(); 
    Vec2 v02 = (p2 - p0).normalized();
    Vec2 v12 = (p2 - p1).normalized();

    float sin_theta_0 = sqrtf(0.5f * (1.0f - LinAlg::dot(v01, v02)));
    float sin_theta_1 = sqrtf(0.5f * (1.0f + LinAlg::dot(v01, v12)));
    float sin_theta_2 = sqrtf(0.5f * (1.0f - LinAlg::dot(v02, v12)));

    const float d_0 = p2.distance_to(p0) * sin_theta_2;
    const float d_1 = p0.distance_to(p1) * sin_theta_0;
    const float d_2 = p1.distance_to(p2) * sin_theta_1;

    // if the distance from any vertex to the opposite side is smaller than t, draw filled triangle
    if(d_0 <= t || d_1 <= t || d_2 <= t) {
        draw_triangle_fill (p0, p1, p2, color);
        return;
    }

    Vec2 p0i = p0 + (v01 + v02).normalized() * t / sin_theta_0;     
    Vec2 p1i = p1 + (v12 - v01).normalized() * t / sin_theta_1; 
    Vec2 p2i = p2 - (v02 + v12).normalized() * t / sin_theta_2; 

    push_quad(
        p0[0], p0[1], p0i[0], p0i[1], 
        p1i[0], p1i[1], p1[0], p1[1], 
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    ); 

    push_quad(
        p1[0], p1[1], p1i[0], p1i[1], 
        p2i[0], p2i[1], p2[0], p2[1], 
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    ); 

    push_quad(
        p2[0], p2[1], p2i[0], p2i[1], 
        p0i[0], p0i[1], p0[0], p0[1], 
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    ); 

}

void SpriteBatch::draw_circle_fill(const Vec2 & center, float radius, const Color & color, int steps) {

    float cx = center[0];
    float cy = center[1];

    for(int i = 0; i < steps; i++) {

        float angle0 = (float) i * Scalar::F_2PI / (float) steps; 
        float angle1 = (float) (i + 1) * Scalar::F_2PI / (float) steps;  

        push_triangle(
            cx, cy, 
            cx + radius * sinf(angle0), cy + radius * cosf(angle0), 
            cx + radius * sinf(angle1), cy + radius * cosf(angle1), 
            0, 0, 0, 0, 0, 0,
            color, 
            {0, 0, 255}
        );
    }
}

void SpriteBatch::draw_circle_line(const Vec2 & center, float radius, float t, const Color & color, int steps) {

    if (t >= radius) {
        draw_circle_fill(center, radius, color);
        return;
    }

    float cx = center[0];
    float cy = center[1];

    for(int i = 0; i < steps; i++) {

        float angle0 = (float) i * Scalar::F_2PI / (float) steps; 
        float angle1 = (float) (i + 1) * Scalar::F_2PI / (float) steps;  

        push_quad(
            cx + (radius - t) * sinf(angle0), cy + (radius - t) * cosf(angle0), 
            cx + radius * sinf(angle0), cy + radius * cosf(angle0), 
            cx + radius * sinf(angle1), cy + radius * cosf(angle1), 
            cx + (radius - t) * sinf(angle1), cy + (radius - t) * cosf(angle1), 
            0, 0, 0, 0, 0, 0, 0, 0,
            color,
            {0, 0, 255}
        ); 
    }
}

// void SpriteBatch::draw_semi_circle_fill(const Vec2 & center, float radius, float radians_start, float radians_end, const Color & color, int steps=15) {

//     float cx = center[0];
//     float cy = center[1];
//     constexpr float pi2 = 2.0f * 3.14159265359f;





// }




void SpriteBatch::draw_line(const Vec2 & start, const Vec2 & end, const Color & color, float t) {

    const Vec2 v = (end - start).normalized();
    const Vec2 normal{0.5f * t * v[1], - 0.5f * t * v[0]};
    
    push_quad(
        start[0] + normal[0], start[1] + normal[1], 
        start[0] - normal[0], start[1] - normal[1], 
        end[0] - normal[0], end[1] - normal[1], 
        end[0] + normal[0], end[1] + normal[1], 
        0, 0, 0, 0, 0, 0, 0, 0,
        color,
        {0, 0, 255}
    ); 
}

inline void SpriteBatch::push_triangle(const float & x0, const float & y0, const float & x1, 
                                       const float & y1, const float & x2, const float & y2, 
                                       const float & uv_x0, const float & uv_y0, 
                                       const float & uv_x1, const float & uv_y1, 
                                       const float & uv_x2, const float & uv_y2, 
                                       const Color & color, const ByteVec3 & cmix) {

    const unsigned int n = (unsigned int) vertices.size();
    indices.insert(indices.end(), { n + 0, n + 2, n + 1 });

    push_vertex(x0, y0, uv_x0, uv_y0, color, cmix);
    push_vertex(x1, y1, uv_x1, uv_y1, color, cmix);
    push_vertex(x2, y2, uv_x2, uv_y2, color, cmix);

}

inline void SpriteBatch::push_quad(const float & x0, const float & y0, const float & x1, const float & y1, 
                                   const float & x2, const float & y2, const float & x3, const float & y3, 
                                   const float & uv_x0, const float & uv_y0, const float & uv_x1, const float & uv_y1, 
                                   const float & uv_x2, const float & uv_y2, const float & uv_x3, const float & uv_y3, 
                                   const Color & color, const ByteVec3 & cmix) {

    const unsigned int n = (unsigned int) vertices.size();
    indices.insert(indices.end(), { n + 0, n + 2, n + 1, n + 0, n + 3,  n + 2 });

    push_vertex(x0, y0, uv_x0, uv_y0, color, cmix);
    push_vertex(x1, y1, uv_x1, uv_y1, color, cmix);
    push_vertex(x2, y2, uv_x2, uv_y2, color, cmix);
    push_vertex(x3, y3, uv_x3, uv_y3, color, cmix);
}

inline void SpriteBatch::push_vertex(const float & x, const float & y, const float & uv_x, const float & uv_y, 
                                     const Color & color, const ByteVec3 & cmix) {
    vertices.push_back({
        {transform[0] * x +  transform[2] * y + transform[4],
         transform[1] * x +  transform[3] * y + transform[5]},
        {uv_x, uv_y}, color, cmix
    });
}

void SpriteBatch::render() {

    glBindVertexArray(vao_id);

    // Binding texture
    if (!current_tex) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        glActiveTexture(GL_TEXTURE0 + current_tex->tex_slot);
        current_tex->bind();
    } 
        
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
    transform_stack.clear();
    transform = Mat3x2::identity;
}