
#include <inttypes.h>
#include <cmath>

#include "auk/numerics/matrix.h"
#include "auk/graphics/color.h"

using namespace auk;

Color::Color(const Vec3 & color) : ByteVec4({(uint8_t) round(255.0f * color[0]), (uint8_t) round(255.0f * color[1]), 
                                             (uint8_t) round(255.0f * color[2]), 255}) {}
Color::Color(const Vec4 & color) : ByteVec4({(uint8_t) round(255.0f * color[0]), (uint8_t) round(255.0f * color[1]), 
                                             (uint8_t) round(255.0f * color[2]), (uint8_t) round(255.0f * color[3])}) {}

float Color::r() const {
    return ((float) data[0]) / 255.0f;
}

float Color::g() const {
    return ((float) data[1]) / 255.0f;
}

float Color::b() const {
    return ((float) data[2]) / 255.0f;
}

float Color::a() const {
    return ((float) data[3]) / 255.0f;
}

const Color Color::black = {0, 0, 0, 255};
const Color Color::white = {255, 255, 255, 255};
const Color Color::red = {255, 0, 0, 255};
const Color Color::green = {0, 255, 0, 255};
const Color Color::blue = {0, 0, 255, 255};