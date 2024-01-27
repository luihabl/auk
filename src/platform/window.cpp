#include "auk/platform/window.h"

#include <SDL.h>

#include "SDL_video.h"
#include "auk/graphics/graphics.h"
#include "auk/platform/log.h"

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

using namespace auk;

struct Window::PlatformImpl {
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
};

Window::Window() : impl(std::make_unique<PlatformImpl>()){};
Window::~Window() = default;

Window& Window::operator=(Window&& other) noexcept {
    impl = std::move(other.impl);
    return *this;
}

Window::Window(const char* name, int w, int h, int x, int y, uint32_t flags, bool use_vsync)
    : impl(std::make_unique<PlatformImpl>()) {
    // Setting up logging so that it has colors on Windows
    Log::setup(Log::Level::Debug);

    // Initializing SDL
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    x = x < 0 ? SDL_WINDOWPOS_UNDEFINED : x;
    y = y < 0 ? SDL_WINDOWPOS_UNDEFINED : y;

    uint32_t wnd_flags = 0;
    if (flags & WindowFlags::Resizable)
        wnd_flags |= SDL_WINDOW_RESIZABLE;
    if (flags & WindowFlags::OpenGL)
        wnd_flags |= SDL_WINDOW_OPENGL;

    impl->window = SDL_CreateWindow(name, x, y, w, h, wnd_flags);

    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    Log::info("SDL %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);

    // Initializing Open GL
    impl->context = SDL_GL_CreateContext(impl->window);
    SDL_GL_MakeCurrent(impl->window, impl->context);
    Graphics::load_functions(SDL_GL_GetProcAddress);
#ifdef DEBUG_OPENGL
    Graphics::setup_debug();
#endif
    SDL_GL_SetSwapInterval(use_vsync);  // using VSync
}

void Window::swap_buffers() {
    SDL_GL_SwapWindow(impl->window);
}

void Window::close() {
    SDL_DestroyWindow(impl->window);
    impl->window = nullptr;
    SDL_GL_DeleteContext(impl->context);
    SDL_Quit();
}

WindowPtr Window::get_window() const {
    return static_cast<WindowPtr>(impl->window);
}

GLContextPtr Window::get_context() const {
    return static_cast<GLContextPtr>(impl->context);
}

void Window::get_drawable_size(int* w, int* h) const {
    SDL_GL_GetDrawableSize(impl->window, w, h);
}

void Window::set_window_title(const char* name) {
    SDL_SetWindowTitle(impl->window, name);
}
