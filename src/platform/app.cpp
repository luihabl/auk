#include "auk/platform/app.h"

#include <chrono>
#include <thread>

auk::App::App(const AppConfig& config) : config(config) {
    wnd = auk::Window(config.name.c_str(), config.w, config.h, auk::Window::UndefinedPos,
                      auk::Window::UndefinedPos,
                      auk::WindowFlags::Resizable | auk::WindowFlags::OpenGL, true);
}

void auk::App::run() {
    auto frame_duration = std::chrono::duration_cast<std::chrono::system_clock::duration>(
        std::chrono::duration<float>{dt()});

    std::chrono::system_clock::time_point next_frame, last_frame;
    next_frame = std::chrono::system_clock::now() + frame_duration;

    auto rnd = SpriteBatch();

    start();

    while (!should_quit) {
        auk::Input::update(wnd, inp);

        std::this_thread::sleep_until(next_frame);
        last_frame = next_frame;
        next_frame += frame_duration;

        auk::Graphics::clear(auk::Color::black);

        update();
        render();

        wnd.swap_buffers();
    }

    end();
}

void auk::App::quit() {
    should_quit = true;
}
float auk::App::dt() {
    return 1.0f / config.fps;
}

float auk::App::fps() {
    return config.fps;
}
