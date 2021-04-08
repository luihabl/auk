
#include <glad/glad.h>

#include "tinysdl/graphics/target.h"
#include "tinysdl/graphics/texture.h"

using namespace TinySDL;

RenderTarget::RenderTarget(int w, int h) {
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    this->tex = Texture::empty(w, h);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->tex.id, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0); //needed if you have more than one draw buffer

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void RenderTarget::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}
