#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include "tinysdl/graphics/texture.h"
#include "tinysdl/platform/log.h"

using namespace TinySDL;

Sprite::Sprite(const char * path) {
    this->data = stbi_load(path, &this->w, &this->h, &this->n_comp, 0);
    if(!data) Log::error("File does not exist: %s", path);
}

Sprite::~Sprite() {
    if(data) free_image_data();
}

void Sprite::free_image_data() {
    stbi_image_free(data);
    data = nullptr;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

Texture::Texture(int w, int h, int n_comp, unsigned char * data) : w(w), h(h){
    glGenTextures(1, &this->id);

    glBindTexture(GL_TEXTURE_2D, this->id);

    GLint format = n_comp == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);

    // add a way to set these parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}


Texture Texture::from_sprite(Sprite * spr) {
    return Texture(spr->w, spr->h, spr->n_comp, spr->data);
}

Texture Texture::from_file(const char * path) {
    Sprite spr(path);
    return Texture(spr.w, spr.h, spr.n_comp, spr.data);
}

Texture Texture::empty(int w, int h) {
       return Texture(w, h, 3, NULL); 
}