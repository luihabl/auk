#include <stdarg.h>
#include <stdio.h>
#ifdef _WIN32
    #include <windows.h>
#endif

#include "tinysdl/core/log.h"

#define LOG_COLOR_RESET   "\033[0m"
#define LOG_COLOR_RED     "\033[31m"     
#define LOG_COLOR_YELLOW  "\033[33m"     
#define LOG_COLOR_BLUE    "\033[34m"

using namespace TinySDL;

namespace {
    Log::Level current_log_level = Log::Level::Debug;
}

void Log::set_level(Log::Level level) {
    current_log_level = level;
}

void Log::setup() {
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    #endif
}

void Log::print(char * msg, ... ) {

    char formatted_msg[LOG_MSG_LEN];
    
    va_list args;
    va_start(args, msg);
    const auto r = vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    va_end(args);

    printf("%s", formatted_msg);
}

void Log::print(char * msg, va_list args ) {
    char formatted_msg[LOG_MSG_LEN];
    const auto r = vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    printf("%s", formatted_msg);
}

void Log::log(Log::Level level, char * msg, ...) {
    if (level > current_log_level) return;

    va_list args;
    va_start(args, msg);
    Log::print(msg, args);
    va_end(args);
}

void Log::debug(char * msg, ...) {
    if (Log::Level::Debug > current_log_level) return;

    char formatted_msg[LOG_MSG_LEN];

    va_list args;
    va_start(args, msg);
    vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    va_end(args);

    printf(LOG_COLOR_BLUE "DEBUG: %s\n" LOG_COLOR_RESET, formatted_msg);
}

void Log::info(char * msg, ...) {
    if (Log::Level::Info > current_log_level) return;

    char formatted_msg[LOG_MSG_LEN];

    va_list args;
    va_start(args, msg);
    vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    va_end(args);

    printf("%s\n", formatted_msg);
}

void Log::warn(char * msg, ...) {
    if (Log::Level::Warning > current_log_level) return;

    char formatted_msg[LOG_MSG_LEN];

    va_list args;
    va_start(args, msg);
    vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    va_end(args);

    printf(LOG_COLOR_YELLOW "WARNING: %s\n" LOG_COLOR_RESET, formatted_msg);
}

void Log::error(char * msg, ...) {
    if (Log::Level::Error > current_log_level) return;

    char formatted_msg[LOG_MSG_LEN];

    va_list args;
    va_start(args, msg);
    vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    va_end(args);

    printf(LOG_COLOR_RED "ERROR: %s\n" LOG_COLOR_RESET, formatted_msg);
}