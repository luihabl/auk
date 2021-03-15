#pragma once 
#include <stdarg.h>

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
 