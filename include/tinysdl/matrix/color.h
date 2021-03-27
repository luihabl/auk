#pragma once 

#include "tinysdl/matrix/matrix.h"
#include "tinysdl/platform/log.h"

namespace TinySDL {
    
    struct Color : Vec<uint8_t, 4> {
        
        using Vec<uint8_t, 4>::Vec;

        Color(int rgb, float a = 1.0f);
        Color(int r, int g, int b, int a = 255);
        Color(float r, float g, float b, float a = 1.0f);
        float r() const, g() const, b() const, a() const;

        static const Color black;
        static const Color white;
    };

}