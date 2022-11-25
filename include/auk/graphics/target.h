#pragma once

#include "auk/graphics/texture.h"
#include <memory>

namespace auk {

    struct RenderTarget {
        RenderTarget(int w, int h);
        ~RenderTarget();

        void begin();
        void end();
        
        Texture* tex = nullptr;
    
        private: 
            unsigned int fbo;
    };

}