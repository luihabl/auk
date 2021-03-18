#include <SDL.h>
#include <glad/glad.h>

#include "tinysdl/graphics/graphics.h"
#include "tinysdl/core/log.h"


using namespace TinySDL;

void Graphics::init() {
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    Log::info("OpenGL %s %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));
}