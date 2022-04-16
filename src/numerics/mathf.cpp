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


// Modeled after the line y = x
float Mathf::Easing::linear_interpolation(float p)
{
	return p;
}

// Modeled after the parabola y = x^2
float Mathf::Easing::quadratic_in(float p)
{
	return p * p;
}

// Modeled after the parabola y = -x^2 + 2x
float Mathf::Easing::quadratic_out(float p)
{
	return -(p * (p - 2));
}

// Modeled after the piecewise quadratic
// y = (1/2)((2x)^2)             ; [0, 0.5)
// y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
float Mathf::Easing::quadratic_in_out(float p)
{
	if(p < 0.5f)
	{
		return 2 * p * p;
	}
	else
	{
		return (-2 * p * p) + (4 * p) - 1;
	}
}

// Modeled after the cubic y = x^3
float Mathf::Easing::cubic_in(float p)
{
	return p * p * p;
}

// Modeled after the cubic y = (x - 1)^3 + 1
float Mathf::Easing::cubic_out(float p)
{
	float f = (p - 1);
	return f * f * f + 1;
}

// Modeled after the piecewise cubic
// y = (1/2)((2x)^3)       ; [0, 0.5)
// y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
float Mathf::Easing::cubic_in_out(float p)
{
	if(p < 0.5)
	{
		return 4 * p * p * p;
	}
	else
	{
		float f = ((2 * p) - 2);
		return 0.5f * f * f * f + 1;
	}
}

// Modeled after the quartic x^4
float Mathf::Easing::quartic_in(float p)
{
	return p * p * p * p;
}

// Modeled after the quartic y = 1 - (x - 1)^4
float Mathf::Easing::quartic_out(float p)
{
	float f = (p - 1);
	return f * f * f * (1 - p) + 1;
}

// Modeled after the piecewise quartic
// y = (1/2)((2x)^4)        ; [0, 0.5)
// y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
float Mathf::Easing::quartic_in_out(float p) 
{
	if(p < 0.5)
	{
		return 8 * p * p * p * p;
	}
	else
	{
		float f = (p - 1);
		return -8 * f * f * f * f + 1;
	}
}

// Modeled after the quintic y = x^5
float Mathf::Easing::quintic_in(float p) 
{
	return p * p * p * p * p;
}

// Modeled after the quintic y = (x - 1)^5 + 1
float Mathf::Easing::quintic_out(float p) 
{
	float f = (p - 1);
	return f * f * f * f * f + 1;
}

// Modeled after the piecewise quintic
// y = (1/2)((2x)^5)       ; [0, 0.5)
// y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
float Mathf::Easing::quintic_in_out(float p) 
{
	if(p < 0.5f)
	{
		return 16 * p * p * p * p * p;
	}
	else
	{
		float f = ((2 * p) - 2);
		return  0.5f * f * f * f * f * f + 1;
	}
}

// Modeled after quarter-cycle of sine wave
float Mathf::Easing::sine_in(float p)
{
	return sinf((p - 1) * Mathf::pi_2) + 1;
}

// Modeled after quarter-cycle of sine wave (different phase)
float Mathf::Easing::sine_out(float p)
{
	return sinf(p * Mathf::pi_2);
}

// Modeled after half sine wave
float Mathf::Easing::sine_in_out(float p)
{
	return 0.5f * (1 - cosf(p * Mathf::pi));
}

// Modeled after shifted quadrant IV of unit circle
float Mathf::Easing::circular_in(float p)
{
	return 1 - sqrtf(1 - (p * p));
}

// Modeled after shifted quadrant II of unit circle
float Mathf::Easing::circular_out(float p)
{
	return sqrtf((2 - p) * p);
}

// Modeled after the piecewise circular function
// y = (1/2)(1 - sqrtf(1 - 4x^2))           ; [0, 0.5)
// y = (1/2)(sqrtf(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
float Mathf::Easing::circular_in_out(float p)
{
	if(p < 0.5f)
	{
		return 0.5f * (1 - sqrtf(1 - 4 * (p * p)));
	}
	else
	{
		return 0.5f * (sqrtf(-((2 * p) - 3) * ((2 * p) - 1)) + 1);
	}
}

// Modeled after the exponential function y = 2^(10(x - 1))
float Mathf::Easing::exponential_in(float p)
{
	return (p == 0.0f) ? p : powf(2, 10 * (p - 1));
}

// Modeled after the exponential function y = -2^(-10x) + 1
float Mathf::Easing::exponential_out(float p)
{
	return (p == 1.0f) ? p : 1 - powf(2, -10 * p);
}

// Modeled after the piecewise exponential
// y = (1/2)2^(10(2x - 1))         ; [0,0.5)
// y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
float Mathf::Easing::exponential_in_out(float p)
{
	if(p == 0.0f || p == 1.0f) return p;
	
	if(p < 0.5f)
	{
		return 0.5f * powf(2, (20 * p) - 10);
	}
	else
	{
		return -0.5f * powf(2, (-20 * p) + 10) + 1;
	}
}

// Modeled after the damped sine wave y = sinf(13pi/2*x)*powf(2, 10 * (x - 1))
float Mathf::Easing::elastic_in(float p)
{
	return sinf(13 * Mathf::pi_2 * p) * powf(2, 10 * (p - 1));
}

// Modeled after the damped sine wave y = sinf(-13pi/2*(x + 1))*powf(2, -10x) + 1
float Mathf::Easing::elastic_out(float p)
{
	return sinf(-13 * Mathf::pi_2 * (p + 1)) * powf(2, -10 * p) + 1;
}

// Modeled after the piecewise exponentially-damped sine wave:
// y = (1/2)*sinf(13pi/2*(2*x))*powf(2, 10 * ((2*x) - 1))      ; [0,0.5)
// y = (1/2)*(sinf(-13pi/2*((2x-1)+1))*powf(2,-10(2*x-1)) + 2) ; [0.5, 1]
float Mathf::Easing::elastic_in_out(float p)
{
	if(p < 0.5f)
	{
		return 0.5f * sinf(13 * Mathf::pi_2 * (2 * p)) * powf(2, 10 * ((2 * p) - 1));
	}
	else
	{
		return 0.5f * (sinf(-13 * Mathf::pi_2 * ((2 * p - 1) + 1)) * powf(2, -10 * (2 * p - 1)) + 2);
	}
}

// Modeled after the overshooting cubic y = x^3-x*sinf(x*pi)
float Mathf::Easing::back_in(float p)
{
	return p * p * p - p * sinf(p * Mathf::pi);
}

// Modeled after overshooting cubic y = 1-((1-x)^3-(1-x)*sinf((1-x)*pi))
float Mathf::Easing::back_out(float p)
{
	float f = (1 - p);
	return 1 - (f * f * f - f * sinf(f * Mathf::pi));
}

// Modeled after the piecewise overshooting cubic function:
// y = (1/2)*((2x)^3-(2x)*sinf(2*x*pi))           ; [0, 0.5)
// y = (1/2)*(1-((1-x)^3-(1-x)*sinf((1-x)*pi))+1) ; [0.5, 1]
float Mathf::Easing::back_in_out(float p)
{
	if(p < 0.5f)
	{
		float f = 2 * p;
		return 0.5f * (f * f * f - f * sinf(f * Mathf::pi));
	}
	else
	{
		float f = (1 - (2*p - 1));
		return 0.5f * (1 - (f * f * f - f * sinf(f * Mathf::pi))) + 0.5f;
	}
}

float Mathf::Easing::bounce_in(float p)
{
	return 1 - bounce_out(1 - p);
}

float Mathf::Easing::bounce_out(float p)
{
	if(p < 4/11.0f)
	{
		return (121 * p * p)/16.0f;
	}
	else if(p < 8/11.0f)
	{
		return (363/40.0f * p * p) - (99/10.0f * p) + 17/5.0f;
	}
	else if(p < 9/10.0f)
	{
		return (4356/361.0f * p * p) - (35442/1805.0f * p) + 16061/1805.0f;
	}
	else
	{
		return (54/5.0f * p * p) - (513/25.0f * p) + 268/25.0f;
	}
}

float Mathf::Easing::bounce_in_out(float p)
{
	if(p < 0.5f)
	{
		return 0.5f * bounce_in(p*2);
	}
	else
	{
		return 0.5f * bounce_out(p * 2 - 1) + 0.5f;
	}
}