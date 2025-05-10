#pragma once

#include "auk/graphics/color.h"
#include "auk/graphics/texture.h"
#include "renderer.h"

namespace auk {

class SpriteBatch {
public:
    struct Vertex {
        auk::Vec2 pos;
        auk::Vec2 uv;
        auk::Color color;
        auk::ByteVec3 cmix;
    };

    explicit SpriteBatch(BufferUsage usage = BufferUsage::Dynamic);

    void set_texture(auk::Texture* tex);

    void push_transform(const auk::Mat3x2& new_transform);
    auk::Mat3x2 pop_transform();

    // Draw texture/sprite
    void draw_tex(const auk::Vec2& pos, const auk::Color& color = auk::Color::white);

    void draw_tex(const auk::Rect& src,
                  const auk::Vec2& pos,
                  const auk::Color& color = auk::Color::white);
    void draw_tex(const auk::Rect& src,
                  const auk::Rect& dst,
                  float rot,
                  bool centered,
                  const auk::Color& color = auk::Color::white);
    void draw_tex(const auk::Rect& src,
                  const auk::Vec2& pos,
                  const auk::Vec2& scale,
                  float rot = 0.0f,
                  bool centered = false,
                  const auk::Color& color = auk::Color::white);

    void draw_tex(const auk::TexRegion& reg,
                  const auk::Vec2& pos,
                  const auk::Color& color = auk::Color::white);
    void draw_tex(const auk::TexRegion& reg, const auk::Color& color = auk::Color::white);

    // Draw rectangle
    void draw_rect_fill(const auk::Rect& rect, const auk::Color& color);
    void draw_rect_line(const auk::Rect& rect, const auk::Color& color, float t);

    // Draw rounded rectangle
    void draw_round_rect_fill(const auk::Rect& rect, float radius, const auk::Color& color);
    void draw_round_rect_line(const auk::Rect& rect,
                              float radius,
                              float t,
                              const auk::Color& color);

    // Draw triangle
    void draw_triangle_fill(const auk::Vec2& p0,
                            const auk::Vec2& p1,
                            const auk::Vec2& p2,
                            const auk::Color& color);
    void draw_triangle_line(const auk::Vec2& p0,
                            const auk::Vec2& p1,
                            const auk::Vec2& p2,
                            float t,
                            const auk::Color& color);

    // Draw circle
    void draw_circle_fill(const auk::Vec2& center,
                          float radius,
                          const auk::Color& color,
                          int steps = 15);
    void draw_circle_line(const auk::Vec2& center,
                          float radius,
                          float t,
                          const auk::Color& color,
                          int steps = 15);

    // Draw semi-circle
    void draw_arc_fill(const auk::Vec2& center,
                       float radius,
                       float radians_start,
                       float radians_end,
                       const auk::Color& color,
                       int steps = 15);
    void draw_arc_line(const auk::Vec2& center,
                       float radius,
                       float radians_start,
                       float radians_end,
                       float t,
                       const auk::Color& color,
                       int steps = 15);

    // Draw line
    void draw_line(const auk::Vec2& start, const auk::Vec2& end, const auk::Color& color, float t);

    // Render everything
    void render();

private:
    Renderer renderer_;

    struct SubBatch {
        unsigned int index_count = 0;
        unsigned int index_offset = 0;
        auk::Texture* tex = nullptr;
    };

    auk::Mat3x2 transform = auk::Mat3x2::identity;
    std::vector<auk::Mat3x2> transform_stack;

    SubBatch sub_batch;
    std::vector<SubBatch> sub_batch_stack;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void new_sub_batch();

    void upload_data();

    void render_sub_batch(const SubBatch& sb);

    void clear();

    void push_vertex(const float& x,
                     const float& y,
                     const float& uv_x,
                     const float& uv_y,
                     const auk::Color& color,
                     const auk::ByteVec3& cmix);

    void push_quad(const float& x0,
                   const float& y0,
                   const float& x1,
                   const float& y1,
                   const float& x2,
                   const float& y2,
                   const float& x3,
                   const float& y3,
                   const float& uv_x0,
                   const float& uv_y0,
                   const float& uv_x1,
                   const float& uv_y1,
                   const float& uv_x2,
                   const float& uv_y2,
                   const float& uv_x3,
                   const float& uv_y3,
                   const auk::Color& color,
                   const auk::ByteVec3& cmix);

    void push_triangle(const float& x0,
                       const float& y0,
                       const float& x1,
                       const float& y1,
                       const float& x2,
                       const float& y2,
                       const float& uv_x0,
                       const float& uv_y0,
                       const float& uv_x1,
                       const float& uv_y1,
                       const float& uv_x2,
                       const float& uv_y2,
                       const auk::Color& color,
                       const auk::ByteVec3& cmix);
};

}  // namespace auk
