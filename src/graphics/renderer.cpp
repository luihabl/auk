#include <glad/glad.h>
#include <cmath>

#include "tinysdl/graphics/renderer.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/graphics/color.h"

#include "tinysdl/numerics/matrix.h"
#include "tinysdl/numerics/rect.h"
#include "tinysdl/numerics/linalg.h"
#include "tinysdl/numerics/mathf.h"


using namespace TinySDL;

BatchRenderer::~BatchRenderer() {
    if (initialized) {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ebo_id);
    }
}

void BatchRenderer::setup() {

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
    clear();

    initialized = true;
}



void BatchRenderer::new_sub_batch() {
    sub_batch_stack.push_back(sub_batch);
    sub_batch.index_offset += sub_batch.index_count;
    sub_batch.index_count = 0;
    sub_batch.tex = nullptr;
}

void BatchRenderer::set_texture(Texture * tex) {
    
    if(tex != sub_batch.tex && sub_batch.index_count > 0 && sub_batch.tex) {
        new_sub_batch();
    } 

    sub_batch.tex = tex;    
}

void BatchRenderer::push_transform(const Mat3x2 & new_transform) {
    transform_stack.push_back(transform);
    transform = LinAlg2D::matmul(new_transform, transform);
}

Mat3x2 BatchRenderer::pop_transform() {
    const Mat3x2 last_transform = transform;
    transform = transform_stack.back();
    transform_stack.pop_back();
    return last_transform;
}

void BatchRenderer::draw_tex(const Rect & src, const Rect & dst, float rot, bool centered, const Color & color) {

    const float w  = src.w;
    const float h  = src.h;

    Vec2 origin = Vec2::zeros;
    if (centered) origin = {w/2.0f, h/2.0f};

    push_transform(LinAlg2D::gen_transform({dst.x, dst.y}, {dst.w / w, dst.h / h}, {0.0f, 0.0f}, rot));

    push_quad(
        0, 0, w, 0, w, h, 0, h,
        src.x / (float) sub_batch.tex->w, src.y / (float) sub_batch.tex->h,
        (src.x + src.w) / (float) sub_batch.tex->w, src.y / (float) sub_batch.tex->h,
        (src.x + src.w) / (float) sub_batch.tex->w, (src.y + src.h) / (float) sub_batch.tex->h,
        src.x / (float) sub_batch.tex->w, (src.y + src.h) / (float) sub_batch.tex->h,
        color,
        {255, 0, 0}
    );

    pop_transform();
}

void BatchRenderer::draw_tex(const Vec2 & pos, const Color & color) {

    const float x = pos[0];
    const float y = pos[1];
    const float w  = (float) sub_batch.tex->w;
    const float h  = (float) sub_batch.tex->h;

    push_quad(
        x, y, x + w, y, x + w, y + h, x, y + h,
        0, 0, 1, 0, 1, 1, 0, 1,
        color,
        {255, 0, 0}
    );
}

void BatchRenderer::draw_tex(const Rect & src, const Vec2 & pos, const Color & color) {

    const float x = pos[0];
    const float y = pos[1];
    const float w  = src.w;
    const float h  = src.h;

    push_quad(
        x, y, x + w, y, x + w, y + h, x, y + h,
        src.x / (float) sub_batch.tex->w, src.y / (float) sub_batch.tex->h,
        (src.x + src.w) / (float) sub_batch.tex->w, src.y / (float) sub_batch.tex->h,
        (src.x + src.w) / (float) sub_batch.tex->w, (src.y + src.h) / (float) sub_batch.tex->h,
        src.x / (float) sub_batch.tex->w, (src.y + src.h) / (float) sub_batch.tex->h,
        color,
        {255, 0, 0}
    );
}

void BatchRenderer::draw_tex(const Rect & src, const Vec2 & pos, const Vec2 & scale, float rot, bool centered, const Color & color) {

    const float w  = src.w;
    const float h  = src.h;
    
    Vec2 origin = Vec2::zeros;
    if (centered) origin = {w/2.0f, h/2.0f};

    push_transform(LinAlg2D::gen_transform(pos, scale, origin, rot));

    push_quad(
        0, 0, w, 0, w, h, 0, h,
        src.x / (float) sub_batch.tex->w, src.y / (float) sub_batch.tex->h,
        (src.x + src.w) / (float) sub_batch.tex->w, src.y / (float) sub_batch.tex->h,
        (src.x + src.w) / (float) sub_batch.tex->w, (src.y + src.h) / (float) sub_batch.tex->h,
        src.x / (float) sub_batch.tex->w, (src.y + src.h) / (float) sub_batch.tex->h,
        color,
        {255, 0, 0}
    );

    pop_transform();
}

void BatchRenderer::draw_tex(const TexRegion & reg, const Vec2 & pos, const Color & color) {

    set_texture(reg.tex);

    const float x = pos[0];
    const float y = pos[1];
    const float w  = reg.w;
    const float h  = reg.h;

    push_quad(
        x, y, x + w, y, x + w, y + h, x, y + h,
        reg.uv[0][0], reg.uv[0][1],
        reg.uv[1][0], reg.uv[1][1],
        reg.uv[2][0], reg.uv[2][1],
        reg.uv[3][0], reg.uv[3][1],
        color,
        {255, 0, 0}
    );
}

void BatchRenderer::draw_tex(const TexRegion & reg, const Color & color) {

    set_texture(reg.tex);

    const float w  = reg.w;
    const float h  = reg.h;

    push_quad(
        0, 0, w, 0, w, h, 0, h,
        reg.uv[0][0], reg.uv[0][1],
        reg.uv[1][0], reg.uv[1][1],
        reg.uv[2][0], reg.uv[2][1],
        reg.uv[3][0], reg.uv[3][1],
        color,
        {255, 0, 0}
    );
}

void BatchRenderer::draw_rect_fill(const Rect & rect, const Color & color) {
    
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

void BatchRenderer::draw_rect_line(const Rect & rect, const Color & color, float t) {
    
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

void BatchRenderer::draw_round_rect_fill(const Rect & rect, float radius, const Color & color) {

    const int steps = 20; //TODO: calculate this or pass as an argument

    const float x = rect.x;
    const float y = rect.y;
    const float w = rect.w;
    const float h = rect.h;

    radius = Mathf::clamp(radius, 0, std::min(0.5f*w, 0.5f*h));
    
    if (radius <= 0) {
        draw_rect_fill(rect, color);
        return;
    }

    draw_arc_fill({x + radius, y + h - radius}, radius, Mathf::rad_left, Mathf::rad_down, color, steps);
    draw_arc_fill({x + w - radius, y + h - radius}, radius, Mathf::rad_down, Mathf::rad_right, color, steps);
    draw_arc_fill({x + w - radius, y + radius}, radius, Mathf::rad_right, Mathf::rad_up, color, steps);
    draw_arc_fill({x + radius, y + radius}, radius, Mathf::rad_up, Mathf::rad_left, color, steps);

    draw_rect_fill({x + radius, y, w - 2.0f * radius, h}, color);
    draw_rect_fill({x, y + radius, radius, h - 2.0f * radius}, color);
    draw_rect_fill({x + w - radius, y + radius, radius, h - 2.0f * radius}, color);
}

void BatchRenderer::draw_round_rect_line(const Rect & rect, float radius, float t, const Color & color) {
    
    const int steps = 20; //TODO: calculate this or pass as an argument

    const float x = rect.x;
    const float y = rect.y;
    const float w = rect.w;
    const float h = rect.h;

    radius = Mathf::clamp(radius, 0, std::min(0.5f*w, 0.5f*h));
    
    if (radius <= 0) {
        draw_rect_line(rect, color, t);
        return;
    }

    if (t >= w || t >= h) {
        draw_round_rect_fill(rect, radius, color);
        return;
    }

    draw_arc_line({x + radius, y + h - radius}, radius, Mathf::rad_left, Mathf::rad_down, t, color, steps);
    draw_arc_line({x + w - radius, y + h - radius}, radius, Mathf::rad_down, Mathf::rad_right, t, color, steps);
    draw_arc_line({x + w - radius, y + radius}, radius, Mathf::rad_right, Mathf::rad_up, t, color, steps);
    draw_arc_line({x + radius, y + radius}, radius, Mathf::rad_up, Mathf::rad_left, t, color, steps);

    draw_rect_fill({x, y + radius, t, h - 2.0f * radius}, color);
    draw_rect_fill({x + radius, y + h - t, w - 2.0f * radius, t}, color);
    draw_rect_fill({x + w - t, y + radius, t, h - 2.0f * radius}, color);
    draw_rect_fill({x + radius, y, w - 2.0f * radius, t}, color);
}


void BatchRenderer::draw_triangle_fill(const Vec2 & p0, const Vec2 & p1, const Vec2 & p2, const Color & color) {

    push_triangle(
        p0[0], p0[1], p1[0], p1[1], p2[0], p2[1],
        0, 0, 0, 0, 0, 0,
        color, 
        {0, 0, 255}
    );
}

void BatchRenderer::draw_triangle_line(const Vec2 & p0, const Vec2 & p1, const Vec2 & p2, float t, const Color & color) {

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

void BatchRenderer::draw_circle_fill(const Vec2 & center, float radius, const Color & color, int steps) {

    float cx = center[0];
    float cy = center[1];

    for(int i = 0; i < steps; i++) {

        float angle0 = (float) i * Mathf::tau / (float) steps; 
        float angle1 = (float) (i + 1) * Mathf::tau / (float) steps;  

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

void BatchRenderer::draw_circle_line(const Vec2 & center, float radius, float t, const Color & color, int steps) {

    if (t >= radius) {
        draw_circle_fill(center, radius, color);
        return;
    }

    float cx = center[0];
    float cy = center[1];

    for(int i = 0; i < steps; i++) {

        float angle0 = (float) i * Mathf::tau / (float) steps; 
        float angle1 = (float) (i + 1) * Mathf::tau / (float) steps;  

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

//Draws filled arc in counter-clockwise direction
void BatchRenderer::draw_arc_fill(const Vec2 & center, float radius, float radians_start, float radians_end, const Color & color, int steps) {

    float cx = center[0];
    float cy = center[1];

    float angle_step = Mathf::delta_angle_counter_clockwise(radians_start, radians_end) / (float) steps;

    for(int i = 0; i < steps; i++) {

        float angle0 = (float) i * angle_step + Mathf::mod(radians_start, Mathf::tau); 
        float angle1 = (float) (i + 1) * angle_step + Mathf::mod(radians_start, Mathf::tau);  

        push_triangle(
            cx, cy, 
            cx + radius * cosf(angle0), cy - radius * sinf(angle0), 
            cx + radius * cosf(angle1), cy - radius * sinf(angle1), 
            0, 0, 0, 0, 0, 0,
            color, 
            {0, 0, 255}
        );
    }
}

//Draws arc in counter-clockwise direction
void BatchRenderer::draw_arc_line(const Vec2& center, float radius, float radians_start, float radians_end, float t, const Color& color, int steps) {

    float cx = center[0];
    float cy = center[1];

    float angle_step = Mathf::delta_angle_counter_clockwise(radians_start, radians_end) / (float)steps;

    for (int i = 0; i < steps; i++) {

        float angle0 = (float)i * angle_step + Mathf::mod(radians_start, Mathf::tau);
        float angle1 = (float)(i + 1) * angle_step + Mathf::mod(radians_start, Mathf::tau);

        push_quad(
            cx + (radius - t) * cosf(angle0), cy - (radius - t) * sinf(angle0),
            cx + radius * cosf(angle0), cy - radius * sinf(angle0),
            cx + radius * cosf(angle1), cy - radius * sinf(angle1),
            cx + (radius - t) * cosf(angle1), cy - (radius - t) * sinf(angle1),
            0, 0, 0, 0, 0, 0, 0, 0,
            color,
            { 0, 0, 255 }
        );

    }
}


void BatchRenderer::draw_line(const Vec2 & start, const Vec2 & end, const Color & color, float t) {

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

inline void BatchRenderer::push_triangle(const float & x0, const float & y0, const float & x1, 
                                       const float & y1, const float & x2, const float & y2, 
                                       const float & uv_x0, const float & uv_y0, 
                                       const float & uv_x1, const float & uv_y1, 
                                       const float & uv_x2, const float & uv_y2, 
                                       const Color & color, const ByteVec3 & cmix) {

    const unsigned int n = (unsigned int) vertices.size();
    indices.insert(indices.end(), { n + 0, n + 2, n + 1 });
    sub_batch.index_count += 3;

    push_vertex(x0, y0, uv_x0, uv_y0, color, cmix);
    push_vertex(x1, y1, uv_x1, uv_y1, color, cmix);
    push_vertex(x2, y2, uv_x2, uv_y2, color, cmix);

}

inline void BatchRenderer::push_quad(const float & x0, const float & y0, const float & x1, const float & y1, 
                                   const float & x2, const float & y2, const float & x3, const float & y3, 
                                   const float & uv_x0, const float & uv_y0, const float & uv_x1, const float & uv_y1, 
                                   const float & uv_x2, const float & uv_y2, const float & uv_x3, const float & uv_y3, 
                                   const Color & color, const ByteVec3 & cmix) {

    const unsigned int n = (unsigned int) vertices.size();
    indices.insert(indices.end(), { n + 0, n + 2, n + 1, n + 0, n + 3,  n + 2 });
    sub_batch.index_count += 6;

    push_vertex(x0, y0, uv_x0, uv_y0, color, cmix);
    push_vertex(x1, y1, uv_x1, uv_y1, color, cmix);
    push_vertex(x2, y2, uv_x2, uv_y2, color, cmix);
    push_vertex(x3, y3, uv_x3, uv_y3, color, cmix);
}

inline void BatchRenderer::push_vertex(const float & x, const float & y, const float & uv_x, const float & uv_y, 
                                     const Color & color, const ByteVec3 & cmix) {
    vertices.push_back({
        {transform[0] * x +  transform[2] * y + transform[4],
         transform[1] * x +  transform[3] * y + transform[5]},
        {uv_x, uv_y}, color, cmix
    });
}

void BatchRenderer::upload_data() {
    glBindVertexArray(vao_id);

    // Uploading vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    // Uploading indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

void BatchRenderer::render_sub_batch(const SubBatch & sb) {
    
    // Binding texture
    if (!sb.tex) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        glActiveTexture(GL_TEXTURE0 + sb.tex->tex_slot);
        sb.tex->bind();
    } 
        
    //Draw everything
    glDrawElements(GL_TRIANGLES, (GLsizei) sb.index_count, GL_UNSIGNED_INT, (void*) (sizeof(unsigned int) * sb.index_offset));
    
}

void BatchRenderer::render() {
    
    upload_data();

    glBindVertexArray(vao_id);
    
    for(const auto& sb: sub_batch_stack)
        render_sub_batch(sb);
        
    render_sub_batch(sub_batch);

    glBindVertexArray(0);

    clear();
}

void BatchRenderer::clear() {

    vertices.clear();
    indices.clear();
    
    transform_stack.clear();
    transform = Mat3x2::identity;
    
    sub_batch_stack.clear();
    sub_batch = SubBatch();

}