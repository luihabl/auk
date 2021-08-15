#pragma once

namespace TinySDL
{

    struct Type {

    private:
        unsigned int id;
        static inline unsigned int type_count = 0;
        
        Type(unsigned int i) { id = i; }

    public: 
        Type() = default;
        
        bool operator== (const Type & other){return id == other.id;}
        bool operator!= (const Type & other){return id != other.id;}

        static unsigned int count() {return type_count;}
        
        template<typename T>
        static Type type_of() {
            static const unsigned int _id = type_count++;
            return Type(_id);    
        }
    };
}
