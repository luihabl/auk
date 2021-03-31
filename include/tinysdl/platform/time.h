#pragma once
#include <inttypes.h>

namespace TinySDL {

    namespace Time {
        uint32_t get_time_ms();
        void wait(uint32_t ms);
    }


    struct SimpleTimer {
        bool is_paused = true;
        uint32_t start_time = 0;
        uint32_t pause_time = 0;

        uint32_t get_time_ms();

        void start();
        void stop();
        void reset();
    };



}