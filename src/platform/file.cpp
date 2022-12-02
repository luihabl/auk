#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define CUTE_ASEPRITE_IMPLEMENTATION
#include <cute_aseprite.h>

#include "auk/platform/file.h"
#include "auk/platform/log.h"

using namespace auk; 

std::string File::load_txt(std::string file_path) {
    std::ifstream in(file_path, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    
    Log::error("File does not exist: %s", file_path.c_str());
    return "";
}

void File::save_png(const std::string& file_path, int w, int h, int comp, unsigned char* data)
{
    stbi_write_png(file_path.c_str(), w, h, comp, data, comp * w * sizeof(unsigned char));
}

unsigned char * File::load_image(std::string file_path, int *w, int *h, int *comp) {
    return stbi_load(file_path.c_str(), w, h, comp, 0);
}

void File::free_image(unsigned char * data) {
    stbi_image_free(data);
}

AsepriteFile* File::load_aseprite(const std::string& file_path)
{
    return cute_aseprite_load_from_file(file_path.c_str(), NULL);
}

void File::free_aseprite(AsepriteFile* file)
{
    cute_aseprite_free(file);
}