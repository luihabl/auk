#include <SDL.h>
#include <glad/glad.h>

#include "tinysdl/graphics/graphics.h"
#include "tinysdl/core/log.h"


using namespace TinySDL;

void Graphics::load_gl_functions() {

    ASSERT_MSG(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "Failed to load GL functions");
    Log::info("OpenGL %s %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));
}