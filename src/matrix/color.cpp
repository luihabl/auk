
#include <inttypes.h>
#include <cmath>

#include "tinysdl/matrix/matrix.h"
#include "tinysdl/matrix/color.h"

using namespace TinySDL;

Color::Color(int r, int g, int b, int a) {
    data = {(uint8_t) r, (uint8_t) g, (uint8_t)  b, (uint8_t) a};
}

Color::Color(float r, float g, float b, float a) {
    data = {(uint8_t) roundf(255.0f * r), (uint8_t) roundf(255.0f * g), 
            (uint8_t) roundf(255.0f * b), (uint8_t) roundf(255.0f * a)};
}

Color::Color(int rgb, float a) {
    // add parsing here
}

float Color::r() {
    return ((float) data[0]) / 255.0f;
}

float Color::g() {
    return ((float) data[1]) / 255.0f;
}

float Color::b() {
    return ((float) data[2]) / 255.0f;
}

float Color::a() {
    return ((float) data[3]) / 255.0f;
}

const Color Color::black = {0, 0, 0, 255};
const Color Color::white = {255, 255, 255, 255};