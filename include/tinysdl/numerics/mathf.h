#pragma once 

#include "matrix.h"

#include <cmath>

// Common math functions and constants
namespace TinySDL 
{
    namespace Mathf
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
        Vec2 approach(Vec2 x, Vec2 target, float delta);

        float lerp(float v0, float v1, float t);
        Vec2 lerp(Vec2 v0, Vec2 v1, float t);


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

        // Adapted from: https://github.com/warrenm/AHEasing
        namespace Easing
        {
            // Linear interpolation (no easing)
            float linear_interpolation(float p);

            // Quadratic easing; p^2
            float quadratic_ease_in(float p);
            float quadratic_ease_out(float p);
            float quadratic_ease_in_out(float p);

            // Cubic easing; p^3
            float cubic_ease_in(float p);
            float cubic_ease_out(float p);
            float cubic_ease_in_out(float p);

            // Quartic easing; p^4
            float quartic_ease_in(float p);
            float quartic_ease_out(float p);
            float quartic_ease_in_out(float p);

            // Quintic easing; p^5
            float quintic_ease_in(float p);
            float quintic_ease_out(float p);
            float quintic_ease_in_out(float p);

            // Sine wave easing; sin(p * PI/2)
            float sine_ease_in(float p);
            float sine_ease_out(float p);
            float sine_ease_in_out(float p);

            // Circular easing; sqrt(1 - p^2)
            float circular_ease_in(float p);
            float circular_ease_out(float p);
            float circular_ease_in_out(float p);

            // Exponential easing, base 2
            float exponential_ease_in(float p);
            float exponential_ease_out(float p);
            float exponential_ease_in_out(float p);

            // Exponentially-damped sine wave easing
            float elastic_ease_in(float p);
            float elastic_ease_out(float p);
            float elastic_ease_in_out(float p);

            // Overshooting cubic easing; 
            float back_ease_in(float p);
            float back_ease_out(float p);
            float back_ease_in_out(float p);

            // Exponentially-decaying bounce easing
            float bounce_ease_in(float p);
            float bounce_ease_out(float p);
            float bounce_ease_in_out(float p);
        }
    } 
}



