#pragma once

#include "auk/graphics/texture.h"

namespace auk {

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