#pragma once 

// Common math functions and constants
namespace TinySDL::Mathf
{
    constexpr float pi = 3.141592653f;
    constexpr float tau = 2.0f * pi;
    constexpr float pi_2 = pi / 2.0f;
    constexpr float pi_4 = pi / 4.0f;

    float mod(float a, float n);

    float delta_angle(float a, float b);
} 



