#pragma once 

#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/matrix/matrix.h"
#include "tinysdl/matrix/color.h"

#include <vector>

namespace TinySDL {

    class SpriteRenderer {
        public:
            SpriteRenderer(Shader & shader);
            ~SpriteRenderer();
            void draw(const Texture & tex, const Vec4 & src_rect, const Vec4 & dst_rect, float rot = 0.0f);
            void draw(const Texture & tex, const Vec2 & pos, float scale = 1.0f, float rot = 0.0f); 
        
        private: 
            Shader shader;
            unsigned int quad_vao_id;
            unsigned int uv_vbo_id;

            void gl_draw_quad();
            void set_uv(const Vec4 & src_rect, const Texture & tex);
            
    };

    struct Vertex {
        Vec2 pos;
        Vec2 uv;
        Color color;
        //Vec<uint8_t, 3> mpar; //Color mixing parameters (0-255)
        Vec<uint8_t, 3> cmix;
    };

    class SpriteBatch {
        public:
            SpriteBatch();
            ~SpriteBatch();
            
            void set_texture(Texture * tex);
            void draw(const Vec4 & src_rect, const Vec4 & dst_rect, float rot = 0.0f, bool centered = false);
            void draw(const Vec4 & src_rect, const Vec2 & pos, const Vec2 & scale, float rot = 0.0f, bool centered = false);
            void render();

        private:
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            unsigned int vao_id;
            unsigned int vbo_id;
            unsigned int ebo_id;

            Texture * current_tex = nullptr;

            void push_vertex(float x, float y, float uv_x, float uv_y, const Mat4x4 & model);
            void push_tex_quad(float w, float h, float tex_w, float tex_h, const Vec4 & src_rect, const Mat4x4 & transform);
          
    };
}