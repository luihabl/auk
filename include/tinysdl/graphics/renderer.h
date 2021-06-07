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
    };

    class SpriteBatch {
        public:
            SpriteBatch();
            void draw(const Texture & tex, const Vec4 & src_rect, const Vec4 & dst_rect, float rot = 0.0f);
            void render();

        private:
            std::vector<Vertex> vertices;
            std::vector<int> indices;

            void push_vertex(const Vertex & vertex);
            


    };
}