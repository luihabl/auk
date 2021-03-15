#pragma once 
#include <iostream> 
#include <memory>
#include <string>
#include <stdexcept>
#include <string>

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

        template<typename ... T>
        extern void print(Log::Level level, const std::string& format, T ... args );
        
        template<typename ... T>
        extern void debug(const std::string& format, T ... args );
        
        template<typename ... T>
        extern void info(const std::string& format, T ... args );
        
        template<typename ... T>
        extern void warn(const std::string& format, T ... args );
        
        template<typename ... T>
        extern void error(const std::string& format, T ... args );

        template<typename ... T>
        extern std::string string_format( const std::string& format, T ... args );
        
        // template<typename ... T>
        // std::string string_format( const std::string& format, T ... args )
        // {
        //     int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
        //     if( size <= 0 ) throw std::runtime_error( "Error during formatting." ); 
            
        //     std::unique_ptr<char[]> buf( new char[ size ] ); 
        //     snprintf( buf.get(), size, format.c_str(), args ... );
            
        //     return std::string( buf.get(), buf.get() + size - 1 );
        // }

        // template<typename ... T>
        // void print(Log::Level level, const std::string& format, T ... args ) {
        //     // if (level <= current_log_level)
        //         std::cout << Log::string_format(format, args ... ) << std::endl;
        // }

        // template<typename ... T>
        // void debug(const std::string& format, T ... args ) {
        //     Log::print(Log::Level::Debug, format, args ... );
        // }

        // template<typename ... T>
        // void info(const std::string& format, T ... args ) {
        //     Log::print(Log::Level::Info, format, args ... );
        // }

        // template<typename ... T>
        // void warn(const std::string& format, T ... args ) {
        //     Log::print(Log::Level::Warning, format, args ... );
        // }

        // template<typename ... T>
        // void error(const std::string& format, T ... args ) {
        //     Log::print(Log::Level::Error, format, args ... );
        // }



    }
} 
 