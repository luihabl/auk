#pragma once

namespace TinySDL
{
    class TypeID {
        
        static inline unsigned int type_count = 0;
    
    public:
        static unsigned int count() {return type_count;}
        
        template<typename T>
        static unsigned int get() {
            static const unsigned int id = type_count++;
            return id;    
        }
    };
}
