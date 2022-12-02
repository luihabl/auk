#pragma once 

#include <string>
#include <cute_aseprite.h>

namespace auk {

    typedef ase_t AsepriteFile;

    namespace File {
        std::string load_txt(std::string file_path);
        
        void save_png(const std::string& file_path, int w, int h, int comp, unsigned char* data);
        unsigned char * load_image(std::string file_path, int *w, int *h, int *comp);
        void free_image(unsigned char * data);

        AsepriteFile* load_aseprite(const std::string& file_path);
        void free_aseprite(AsepriteFile* f);
    }

}