#pragma once

#include <functional>

#include "auk/input/keys.h"
#include "auk/numerics/matrix.h"

namespace auk {

struct InputEvent {
    virtual void* event() = 0;
};

struct InputHandler {
    std::function<void()> on_quit;
    std::function<void(int, int)> on_window_resize;
    std::function<void(InputEvent*)> on_event;
};

class VirtualInput;

namespace Input {

bool just_pressed(Key k);
bool pressed(Key k);
bool released(Key k);

auk::IVec2 get_mouse_window_pos();
auk::IVec2 get_mouse_global_pos();

void register_button(VirtualInput* button);
void unregister_button(VirtualInput* button);
size_t reg_size();

void update(InputHandler& handler);
}  // namespace Input
}  // namespace auk
