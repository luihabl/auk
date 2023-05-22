#pragma once

#include <auk.h>
#include <functional>

namespace auk
{
    struct InputHandler {
        std::function<void()> on_quit;
        std::function<void(int, int)> on_window_resize;
        std::function<void(SDL_Event&)> on_event;
    };

    class VirtualInput;

    namespace Input {

        bool just_pressed(Key k);
        bool pressed(Key k);
        bool released(Key k);

        auk::IVec2 get_mouse_window_pos();
        auk::IVec2 get_mouse_global_pos();
        
        void register_button(VirtualInput * button);
        void unregister_button(VirtualInput * button);
        size_t reg_size();

        void update(InputHandler & handler);
    }
} 
