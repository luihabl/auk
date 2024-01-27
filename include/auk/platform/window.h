#pragma once

#include <cstdint>
#include <memory>

namespace auk {

using WindowPtr = void*;
using GLContextPtr = void*;

namespace WindowFlags {
static constexpr uint32_t Resizable = 1 << 0;
static constexpr uint32_t OpenGL = 1 << 1;
}  // namespace WindowFlags

class Window {
public:
    static constexpr int UndefinedPos = -1;

    Window();
    ~Window();
    Window& operator=(Window&& other) noexcept;

    Window(const char* name,
           int w,
           int h,
           int x = UndefinedPos,
           int y = UndefinedPos,
           uint32_t flags = WindowFlags::Resizable | WindowFlags::OpenGL,
           bool use_vsync = false);

    void close();
    void swap_buffers();
    void get_drawable_size(int* w, int* h) const;
    WindowPtr get_window() const;
    GLContextPtr get_context() const;

    void set_window_title(const char* name);

private:
    struct PlatformImpl;
    std::unique_ptr<PlatformImpl> impl;
};
}  // namespace auk
