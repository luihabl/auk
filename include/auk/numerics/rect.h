#pragma once

#include "auk/numerics/matrix.h"

namespace auk{

    template <typename T>
    struct TRect {
        
        T x = 0, y = 0;
        T w = 0, h = 0;

        TRect() = default;
        TRect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
        TRect(const Vec4 & v) : x(v[0]), y(v[1]), w(v[2]), h(v[3]) {}

        operator Vec4() const {return Vec4{x, y, w, h};}
        
        bool overlaps(const TRect & o) const {
            return  (x < (o.x + o.w)) && ((x + w) > o.x) &&
                    (y < (o.y + o.h)) && ((y + h) > o.y);
        }

        bool contains(const Vec<T, 2>& point) {
            return (    point[0] >= x && point[0] < (x + w)
                    &&  point[1] >= y && point[1] < (y + h));
        }

        TRect operator+(const Vec<T, 2> & vec) {
            return {x + vec[0] , y + vec[1], w, h};
        }

        template <typename G>
        TRect<G> cast() const {
            return {(G) x, (G) y, (G) w, (G) h};
        }

        Vec<T, 2> pos() {
            return Vec<T, 2>({x, y});
        }
    };

    typedef TRect<float> Rect;
    typedef TRect<int> IntRect;

}
