#pragma once

#include "tinysdl/matrix/matrix.h"

namespace TinySDL{

    template <typename T>
    struct TRect {
        
        T x = 0, y = 0;
        T w = 0, h = 0;

        TRect() = default;
        TRect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
        TRect(const Vec4 & v) : x(v[0]), y(v[1]), w(v[2]), h(v[3]) {}

        operator Vec4() const {return Vec4{x, y, w, h};}
        
    };

    typedef TRect<float> Rect;
    typedef TRect<int> IntRect;

}
