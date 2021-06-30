#include <cmath>

#include "tinysdl/numerics/mathf.h"

using namespace TinySDL;

float Mathf::mod(float a, float n) {
    return a - (int)(a / n) * n;
}

float Mathf::delta_angle(float a, float b) {
    return mod((b - a) + pi, tau) - pi;
}