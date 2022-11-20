#pragma once 

#include "auk/numerics/matrix.h"

namespace auk {
    
    struct Color : ByteVec4 {
        
        Color() = default;
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : ByteVec4({r, g, b, a}){}
        Color(const Vec3 & color);
        Color(const Vec4 & color);

        float r() const, g() const, b() const, a() const;

        operator Vec3() const {return Vec3{r(), g(), b()};}
        operator Vec4() const {return Vec4{r(), g(), b(), a()};}

        static const Color black;
        static const Color white;
        static const Color red;
        static const Color blue;
        static const Color green;
    };

}