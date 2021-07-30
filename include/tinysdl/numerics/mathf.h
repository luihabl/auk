#pragma once 

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

    float clamp(float v, float v_min, float v_max);

    float delta_angle_min(float a, float b);
    float delta_angle_counter_clockwise(float radians_start, float radians_end);

    float sign(float v);
    
} 



