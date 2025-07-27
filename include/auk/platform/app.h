#pragma once

#include "auk.h"

namespace auk {
struct AppConfig {
    int w, h;
    float fps;
    std::string name;
};

class App {
public:
    App() = default;

    App(const AppConfig& config);
    void run();
    void quit();

    virtual void start() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void end() = 0;
    virtual ~App() {}

    auk::InputHandler inp;
    auk::Window wnd;

    float dt();
    float fps();

protected:
    bool should_quit = false;
    AppConfig config;
};
}  // namespace auk