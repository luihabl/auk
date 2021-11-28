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

Vec2 Mathf::approach(Vec2 x, Vec2 target, float delta)
{
    if ((target - x).length() < delta)
        return target;

    return x + (target - x).normalized() * delta;
}

float Mathf::lerp(float v0, float v1, float t)
{
    return (1.0f - t) * v0 + t * v1;
}

Vec2 Mathf::lerp(Vec2 v0, Vec2 v1, float t)
{
    return (1.0f - t) * v0 + t * v1;
}