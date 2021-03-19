#pragma once 
#include <stdarg.h>
#include <stdlib.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"     
#define COLOR_YELLOW  "\033[33m"     
#define COLOR_BLUE    "\033[34m"     

#define LOG_MSG_LEN 1024
#define LOG_ERROR(msg) Log::error("%s (%d): " msg, __FILE__, __LINE__);
#define LOG_WARN(msg) Log::warn("%s (%d): " msg, __FILE__, __LINE__);
#define ASSERT(check, msg) if(!check) {LOG_ERROR(msg); abort();} 

namespace TinySDL {
    namespace Log {

        enum class Level : int {
            Nothing,
            Error,
            Warning,
            Info,
            Debug
        };

        void set_level(Level level);

        void print(char * msg, va_list args);
        void print(char * msg, ...);
        
        void log(Level level, char * msg, ...);
        void debug(char * msg, ...);
        void info(char * msg, ...);
        void warn(char * msg, ...);
        void error(char * msg, ...);

    }
} 
 