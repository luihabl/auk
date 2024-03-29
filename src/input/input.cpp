#include "auk/input/input.h"

#include <auk.h>

#include <cstring>
#include <list>
#include <vector>

#include "SDL_events.h"
#include "SDL_video.h"

using namespace auk;

struct SDLInputEvent : public auk::InputEvent {
    SDLInputEvent(SDL_Event* sdl_event) : event_(sdl_event) {}
    void* event() override { return event_; }
    SDL_Event* event_ = nullptr;
};

namespace {
int n_keys = 512;
uint8_t* current_keyboard_state = (uint8_t*)SDL_GetKeyboardState(&n_keys);
std::vector<uint8_t> previous_keyboard_state(n_keys);

IVec2 mouse_global_position = IVec2::zeros;
IVec2 mouse_window_position = IVec2::zeros;

// IVec2 window_size = IVec2::zeros;
IVec2 window_position = IVec2::zeros;

std::list<VirtualInput*> registered_buttons;
}  // namespace

void Input::update(const Window& wnd, InputHandler& handler) {
    std::memcpy(&(previous_keyboard_state[0]), current_keyboard_state, sizeof(uint8_t) * n_keys);

    SDL_GetWindowPosition((SDL_Window*)wnd.get_window(), &window_position[0], &window_position[1]);
    SDL_GetGlobalMouseState(&mouse_global_position[0], &mouse_global_position[1]);
    mouse_window_position = mouse_global_position - window_position;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (handler.on_event) {
            auto sdl_event = SDLInputEvent(&event);
            handler.on_event(&sdl_event);
        }

        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int w, h;

                wnd.get_drawable_size(&w, &h);
                handler.on_window_resize(w, h);
            }
        }

        if (event.type == SDL_QUIT) {
            handler.on_quit();
        }
    }

    for (auto& b : registered_buttons) {
        b->update();
    }
}

size_t Input::reg_size() {
    return registered_buttons.size();
}

bool Input::pressed(Key k) {
    return current_keyboard_state[(int)k];
}

bool Input::just_pressed(Key k) {
    return current_keyboard_state[(int)k] && !previous_keyboard_state[(int)k];
}

bool Input::released(Key k) {
    return !current_keyboard_state[(int)k] && previous_keyboard_state[(int)k];
}

IVec2 Input::get_mouse_window_pos() {
    return mouse_window_position;
}

IVec2 Input::get_mouse_global_pos() {
    return mouse_global_position;
}

void Input::register_button(VirtualInput* button) {
    registered_buttons.emplace_back(button);
}

void Input::unregister_button(VirtualInput* button) {
    registered_buttons.remove(button);
}
