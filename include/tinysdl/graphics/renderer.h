#pragma once 

#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/matrix/matrix.h"
#include "tinysdl/matrix/rect.h"
#include "tinysdl/matrix/color.h"

#include <vector>

namespace TinySDL {
    
    struct Vertex {
        Vec2 pos;
        Vec2 uv;
        Color color;
        ByteVec3 cmix;
    };

    class SpriteBatch {
        public:
            SpriteBatch();
            ~SpriteBatch();
            
            void set_texture(Texture * tex);
            
            void push_transform(const Mat3x2 & new_transform);
            Mat3x2 pop_transform();

            void draw(const Rect & src, const Vec2 & pos);
            void draw(const Rect & src, const Rect & dst, float rot, bool centered);
            void draw(const Rect & src, const Vec2 & pos, const Vec2 & scale, float rot = 0.0f, bool centered = false);
            void draw_rect_fill(const Rect & rect, const Color & color);
            void draw_rect_line(const Rect & rect, const Color & color, const float & t);
            void draw_triangle_fill(const Vec2 & p0, const Vec2 & p1, const Vec2 & p2, const Color & color);
            void draw_circle_fill(const Vec2 & center, const float & radius, const Color & color);

            void render();

        private:
            Mat3x2 transform;
            std::vector<Mat3x2> transform_stack;

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            unsigned int vao_id;
            unsigned int vbo_id;
            unsigned int ebo_id;

            Texture * current_tex = nullptr;

            void push_vertex(const float & x, const float & y, const float & uv_x, const float & uv_y, const Color & color, const ByteVec3 & cmix);
            void push_tex_quad(float w, float h, float tex_w, float tex_h, const Vec4 & src_rect);
            
            void push_quad(const float & x0, const float & y0, const float & x1, const float & y1, 
                           const float & x2, const float & y2, const float & x3, const float & y3, 
                           const float & uv_x0, const float & uv_y0, const float & uv_x1, const float & uv_y1, 
                           const float & uv_x2, const float & uv_y2, const float & uv_x3, const float & uv_y3, 
                           const Color & color, const ByteVec3 & cmix);
            
            void push_triangle(const float & x0, const float & y0, const float & x1, 
                               const float & y1, const float & x2, const float & y2, 
                               const float & uv_x0, const float & uv_y0, 
                               const float & uv_x1, const float & uv_y1, 
                               const float & uv_x2, const float & uv_y2, 
                               const Color & color, const ByteVec3 & cmix);
    };
}