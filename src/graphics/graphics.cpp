#include <SDL.h>
#include <glad/glad.h>

#include "tinysdl/graphics/graphics.h"
#include "tinysdl/core/log.h"

#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 6

using namespace TinySDL;

void Graphics::init() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    Log::info("OpenGL %s %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));
}