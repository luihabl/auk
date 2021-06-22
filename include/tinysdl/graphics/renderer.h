#pragma once 

#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/matrix/matrix.h"
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
            //void push_transform(const Mat4x4 & m);
            // void pop_transform();
            
            void draw(const Vec4 & src_rect, const Vec4 & dst_rect, float rot = 0.0f, bool centered = false);
            void draw(const Vec4 & src_rect, const Vec2 & pos, const Vec2 & scale, float rot = 0.0f, bool centered = false);
            void draw_rect_fill(const Vec4 & rect, const Color & color);
            void draw_rect_line(const Vec4 & rect, const Color & color, const float & t);

            void render();

        private:
            // Mat4x4 transform;

            
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            unsigned int vao_id;
            unsigned int vbo_id;
            unsigned int ebo_id;

            Texture * current_tex = nullptr;

            void push_vertex(const float & x, const float & y, const float & uv_x, const float & uv_y, const Mat4x4 & model, const Color & color, const ByteVec3 & cmix);
            void push_tex_quad(float w, float h, float tex_w, float tex_h, const Vec4 & src_rect, const Mat4x4 & transform);
            void push_quad(const float & x0, const float & y0, const float & x1, const float & y1, 
                           const float & x2, const float & y2, const float & x3, const float & y3, 
                           const float & uv_x0, const float & uv_y0, const float & uv_x1, const float & uv_y1, 
                           const float & uv_x2, const float & uv_y2, const float & uv_x3, const float & uv_y3, 
                           const Color & color, const ByteVec3 & cmix, const Mat4x4 & model);
    };
}