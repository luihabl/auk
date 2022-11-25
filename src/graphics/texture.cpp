#include <glad/glad.h>

#include "auk/graphics/texture.h"
#include "auk/platform/log.h"
#include "auk/platform/file.h"

using namespace auk;

Image::Image(const char * path) {
    this->data = File::load_image(path, &this->w, &this->h, &this->n_comp);
    if(!data) Log::error("File does not exist: %s", path);
}

Image::~Image() {
    if(data) free_image_data();
}

void Image::free_image_data() {
    File::free_image(data);
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

    full_rect = {0, 0, (float) w, (float) h};

    // add a way to set these parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}


Texture* Texture::from_sprite(Image * spr) {
    return new Texture(spr->w, spr->h, spr->n_comp, spr->data);
}

Texture* Texture::from_file(const char * path) {
    Image spr(path);
    return new Texture(spr.w, spr.h, spr.n_comp, spr.data);
}

Texture* Texture::empty(int w, int h) {
       return new Texture(w, h, 4, NULL); 
}