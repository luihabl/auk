#pragma once
#include <inttypes.h>

namespace auk {

namespace Time {
uint32_t get_time_ms();
float get_time_high_precision_ms();

void wait(uint32_t ms);
}  // namespace Time

struct SimpleTimer {
    bool is_paused = true;
    uint32_t start_time = 0;
    uint32_t pause_time = 0;

    uint32_t get_time_ms();

    void start();
    void stop();
    void reset();
};

}  // namespace auk