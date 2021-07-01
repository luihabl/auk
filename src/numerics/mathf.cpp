#include <cmath>

#include "tinysdl/numerics/mathf.h"

using namespace TinySDL;

float Mathf::mod(float a, float b) {
    return a - floorf(a / b) * b;
}

float Mathf::delta_angle(float a, float b) {
    return mod((b - a) + pi, tau) - pi;
}

float Mathf::clamp(float v, float v_min, float v_max) {
  const float t = v < v_min ? v_min : v;
  return t > v_max ? v_max : t;
}

