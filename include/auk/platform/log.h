#pragma once 
#include <stdarg.h>
#include <stdlib.h>

#define LOG_MSG_LEN 1024
#define LOG_ERROR(msg) Log::error("%s (%d): " msg, __FILE__, __LINE__);
#define LOG_WARN(msg) Log::warn("%s (%d): " msg, __FILE__, __LINE__);
#define ASSERT_MSG(check, msg) if(!check) {LOG_ERROR(msg); abort();} 


namespace auk {
    namespace Log {

        enum class Level : int {
            Nothing,
            Error,
            Warning,
            Info,
            Debug
        };

        void set_level(Level level);
        void setup(Level level = Level::Info);

        void print(const char * msg, va_list args);
        void print(const char * msg, ...);
        
        void log(Level level, const char * msg, ...);
        void debug(const char * msg, ...);
        void info(const char * msg, ...);
        void warn(const char * msg, ...);
        void error(const char * msg, ...);

    }
} 
 