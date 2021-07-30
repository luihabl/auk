#include <cmath>

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

float Mathf::clamp(float v, float v_min, float v_max) {
  const float t = v < v_min ? v_min : v;
  return t > v_max ? v_max : t;
}

float Mathf::sign(float v) {
    if(v == 0) return 0;
    else return v / abs(v);
}
