#pragma once 

#include "tinysdl/graphics/shader.h"
#include "tinysdl/graphics/texture.h"
#include "tinysdl/matrix/matrix.h"
#include "tinysdl/matrix/color.h"

namespace TinySDL {

    struct SpriteRenderer {
        SpriteRenderer(Shader & shader);
        ~SpriteRenderer();
        void draw(const Texture & tex, const Vec2 & pos, const Vec2 & size, float rot = 0.0f, const Color & color = Color::white);

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
        
        Texture target;
    
        private: 

            unsigned int fbo;
            unsigned int quad_vao;
            void init(int w, int h);
    };

}