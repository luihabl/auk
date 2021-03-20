#include <string>
#include <fstream>

#include "tinysdl/core/file.h"
#include "tinysdl/core/log.h"

using namespace TinySDL; 

std::string File::read(std::string file_path) {
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