#pragma once 

#include <string>

namespace TinySDL {

    namespace File {
        std::string load_txt(std::string file_path);
        
        unsigned char * load_image(std::string file_path, int *w, int *h, int *comp);
        void free_image(unsigned char * data);
    }

}