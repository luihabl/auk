#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include "tinysdl/graphics/texture.h"
#include "tinysdl/core/log.h"

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

void SpriteTexture::bind() {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

SpriteTexture SpriteTexture::from_sprite(Sprite * spr) {
    SpriteTexture spr_tex;
    spr_tex.w = spr->w;
    spr_tex.h = spr->h;

    glGenTextures(1, &spr_tex.id);

    glBindTexture(GL_TEXTURE_2D, spr_tex.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spr->w, spr->h, 0, GL_RGB, GL_UNSIGNED_BYTE, spr->data);

    // add a way to set these parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    return spr_tex;
}

SpriteTexture SpriteTexture::from_file(const char * path) {
    Sprite spr(path);
    return SpriteTexture::from_sprite(&spr);
}

TargetTexture::TargetTexture(int w, int h) {
    
    glGenFramebuffers(1, &this->fbo_id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo_id);

    glGenTextures(1, &this->id); 
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->id, 0);
}

void TargetTexture::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo_id);
    glViewport(0, 0, this->w, this->h);
}
