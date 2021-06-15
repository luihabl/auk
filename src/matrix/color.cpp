
#include <inttypes.h>
#include <cmath>

#include "tinysdl/matrix/matrix.h"
#include "tinysdl/matrix/color.h"

using namespace TinySDL;

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : Vec<uint8_t, 4>({r, g, b, a}){}

// Color::Color(float r, float g, float b, float a) : Vec<uint8_t, 4>({(uint8_t) roundf(255.0f * r), (uint8_t) roundf(255.0f * g), 
//                                                    (uint8_t) roundf(255.0f * b), (uint8_t) roundf(255.0f * a)}) {}

// Color::Color(int rgb, float a) {
//     // add parsing here
// }

float Color::r() const {
    return ((float) _Elems[0]) / 255.0f;
}

float Color::g() const {
    return ((float) _Elems[1]) / 255.0f;
}

float Color::b() const {
    return ((float) _Elems[2]) / 255.0f;
}

float Color::a() const {
    return ((float) _Elems[3]) / 255.0f;
}

const Color Color::black = {0, 0, 0, 255};
const Color Color::white = {255, 255, 255, 255};