
#include <SDL.h>
#include "tinysdl/platform/time.h"

using namespace TinySDL;

uint32_t Time::get_time_ms() {
    return SDL_GetTicks();
}

float Time::get_time_high_precision_ms() {
    return 1000.0f * (float) SDL_GetPerformanceCounter() / (float) SDL_GetPerformanceFrequency();
}

void Time::wait(uint32_t ms) {
    SDL_Delay(ms);
}

uint32_t SimpleTimer::get_time_ms() {
    if (is_paused) return pause_time;
    else return Time::get_time_ms() - start_time;
}

void SimpleTimer::start() {
    is_paused = false;
    start_time = Time::get_time_ms() - pause_time;
    pause_time = 0;
}

void SimpleTimer::stop() {
    is_paused = true;
    pause_time = Time::get_time_ms() - start_time;
    start_time = 0;
}

void SimpleTimer::reset() {
    is_paused = true;
    start_time = 0;
    pause_time = 0;
}


