#pragma once 

#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/math/matrix.h"

namespace TinySDL {

    struct SpriteRenderer {
        SpriteRenderer(Shader & shader);
        ~SpriteRenderer();
        void draw(Texture & tex, Vec2 pos, Vec2 size = (Vec2::ones() * 10.0f), float rot = 0.0f, Vec3 color = Vec3::ones());

        private: 
            Shader shader;
            unsigned int quad_vao;
            void init();
    };

    struct TargetRenderer {
        TargetRenderer() = default;
        TargetRenderer(int w, int h);
        ~TargetRenderer();
        void begin();
        void end();
        void draw();
    
        private: 
            Texture target;
            unsigned int fbo;
            unsigned int quad_vao;
            void init(int w, int h);
    };

}