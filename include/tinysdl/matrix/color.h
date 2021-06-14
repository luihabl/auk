#pragma once 

#include "tinysdl/matrix/matrix.h"
#include "tinysdl/platform/log.h"

namespace TinySDL {
    
    struct Color : Vec<uint8_t, 4> {
        
        Color() = default;
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        // Color(int rgb, float a = 1.0f);
        // Color(float r, float g, float b, float a = 1.0f);

        float r() const, g() const, b() const, a() const;

        operator Vec3() const {return Vec3{r(), g(), b()};}
        operator Vec4() const {return Vec4{r(), g(), b(), a()};}

        static const Color black;
        static const Color white;
    };

}