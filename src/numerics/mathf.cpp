#include <cmath>
#include <algorithm>

#include "tinysdl/numerics/mathf.h"

using namespace TinySDL;

float Mathf::mod(float a, float b) {
    return a - floorf(a / b) * b;
}

float Mathf::delta_angle_min(float a, float b) {
    return mod((b - a) + pi, tau) - pi;
}

float Mathf::delta_angle_counter_clockwise(float radians_start, float radians_end) {
    return Mathf::mod(radians_end - radians_start, Mathf::tau);
}

float Mathf::approach(float x, float target, float delta) {
    if (x < target)
        return std::min(x + delta, target);
    else
        return std::max(x - delta, target);
}