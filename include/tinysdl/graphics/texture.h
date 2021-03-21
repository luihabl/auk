#pragma once

namespace TinySDL {

    struct Sprite {
        Sprite() = default;
        Sprite(const char * path);
        ~Sprite();
        void free_image_data();

        unsigned char * data = nullptr;
        int w = 0, h = 0, n_comp = 0;        
    };

    class SpriteTexture {
        public:
            SpriteTexture() = default;
            unsigned int id;
            int w = 0, h = 0;

            void bind();
            static SpriteTexture from_file(const char * path);
            static SpriteTexture from_sprite(Sprite * spr);
    };


}