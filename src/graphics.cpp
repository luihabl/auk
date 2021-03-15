#include <graphics.h>
#include <log.h>

#include <SDL.h>
#include <glad/glad.h>


using namespace TinySDL;

void Graphics::init() {
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    Log::info("OpenGL %s %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));
}