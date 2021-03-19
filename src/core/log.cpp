#include <stdarg.h>
#include <stdio.h>

#include "tinysdl/core/log.h"

using namespace TinySDL;

namespace {
    Log::Level current_log_level;
}

void Log::set_level(Log::Level level) {
    current_log_level = level;
}


void Log::print(char * msg, ... ) {

    char formatted_msg[LOG_MSG_LEN];
    
    va_list args;
    va_start(args, msg);
    const auto r = vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    va_end(args);

    printf("%s\n", formatted_msg);
}

void Log::print(char * msg, va_list args ) {
    char formatted_msg[LOG_MSG_LEN];
    const auto r = vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    printf("%s\n", formatted_msg);
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

    printf(COLOR_BLUE "DEBUG: %s\n" COLOR_RESET, formatted_msg);
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

    printf(COLOR_YELLOW "WARNING: %s\n" COLOR_RESET, formatted_msg);
}

void Log::error(char * msg, ...) {
    if (Log::Level::Error > current_log_level) return;

    char formatted_msg[LOG_MSG_LEN];

    va_list args;
    va_start(args, msg);
    vsnprintf(formatted_msg, sizeof(formatted_msg), msg, args);
    va_end(args);

    printf(COLOR_RED "ERROR: %s\n" COLOR_RESET, formatted_msg);
}