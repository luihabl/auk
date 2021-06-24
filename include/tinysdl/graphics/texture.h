#pragma once

#include "tinysdl/matrix/rect.h"

namespace TinySDL {

    struct Sprite {
        Sprite() = default;
        Sprite(const char * path);
        ~Sprite();
        void free_image_data();

        unsigned char * data = nullptr;
        int w = 0, h = 0, n_comp = 0;        
    };

    struct Texture {

        Texture() = default;
        Texture(int w, int h, int n_comp, unsigned char * data);
        
        unsigned int id;
        unsigned int tex_slot = 0;
        
        int w = 0, h = 0;
        Rect full_rect;

        void bind() const;
        static Texture from_file(const char * path);
        static Texture from_sprite(Sprite * spr);
        static Texture empty(int w, int h);
    };
}