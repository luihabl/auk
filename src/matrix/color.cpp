
#include <inttypes.h>
#include <cmath>

#include "tinysdl/matrix/matrix.h"
#include "tinysdl/matrix/color.h"

using namespace TinySDL;

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
const Color Color::red = {255, 0, 0, 255};
const Color Color::green = {0, 255, 0, 255};
const Color Color::blue = {0, 0, 255, 255};