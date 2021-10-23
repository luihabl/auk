#pragma once 

#include <cmath>

// Common math functions and constants
namespace TinySDL::Mathf
{
    constexpr float pi = 3.141592653f;
    constexpr float tau = 2.0f * pi;
    constexpr float pi_2 = pi / 2.0f;
    constexpr float pi_4 = pi / 4.0f;
    
    constexpr float rad_right = 0.0f;
    constexpr float rad_up = pi / 2.0f;
    constexpr float rad_left = pi;
    constexpr float rad_down = 3.0f * pi / 2.0f;


    float mod(float a, float n);

    float approach(float x, float target, float delta);

    float delta_angle_min(float a, float b);
    float delta_angle_counter_clockwise(float radians_start, float radians_end);

    
    template<typename T>
    inline T sign(T v) {
        if(v == 0) return 0;
        else return v / abs(v);
    }

    template <typename T>
    inline T clamp(T v, T v_min, T v_max) {
        const T t = v < v_min ? v_min : v;
        return t > v_max ? v_max : t;
    }

} 



