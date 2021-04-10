#pragma once

#include "tinysdl/matrix/matrix.h"

namespace TinySDL {

    struct Sprite {
        Sprite() = default;
        Sprite(const char * path);
        ~Sprite();
        void free_image_data();

        unsigned char * data = nullptr;
        int w = 0, h = 0, n_comp = 0;        
    };

    class Texture {
        public:
            Texture() = default;
            Texture(int w, int h, int n_comp, unsigned char * data);
            unsigned int id;
            int w = 0, h = 0;
            Vec4 full_rect;

            void bind() const;
            static Texture from_file(const char * path);
            static Texture from_sprite(Sprite * spr);
            static Texture empty(int w, int h);
    };
}