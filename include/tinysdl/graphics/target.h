#pragma once

#include "tinysdl/graphics/texture.h"

namespace TinySDL {

    struct RenderTarget {
        RenderTarget() = default;
        RenderTarget(int w, int h);
        void begin();
        void end();
        
        Texture tex;
    
        private: 
            unsigned int fbo;
    };

}