#include <log.h>
#include <iostream> 
#include <memory>
#include <string>
#include <stdexcept>

using namespace TinySDL;

namespace {
    Log::Level current_log_level;
}

void Log::set_level(Log::Level level) {
    current_log_level = level;
}

template<typename ... T>
void Log::print(Log::Level level, const std::string& format, T ... args ) {
    if (level <= current_log_level)
        std::cout << Log::string_format(format, args ... ) << std::endl;
}

template<typename ... T>
void Log::debug(const std::string& format, T ... args ) {
    Log::print(Log::Level::Debug, format, args ... );
}

template<typename ... T>
void Log::info(const std::string& format, T ... args ) {
    Log::print(Log::Level::Info, format, args ... );
}

template<typename ... T>
void Log::warn(const std::string& format, T ... args ) {
    Log::print(Log::Level::Warning, format, args ... );
}

template<typename ... T>
void Log::error(const std::string& format, T ... args ) {
    Log::print(Log::Level::Error, format, args ... );
}


template<typename ... T>
std::string Log::string_format( const std::string& format, T ... args )
{
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    if( size <= 0 ) throw std::runtime_error( "Error during formatting." ); 
    
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    
    return std::string( buf.get(), buf.get() + size - 1 );
}