#pragma once

#include "auk.h"
#include "input.h"
#include "keys.h"

namespace auk {

class VirtualInput {
public:
    virtual void update() = 0;
    virtual VirtualInput& operator=(const VirtualInput& other) {
        registered = registered || other.registered;
        return *this;
    }

    virtual VirtualInput& register_input() {
        if (!registered) {
            registered = true;
            Input::register_button(this);
        }

        return *this;
    }

    virtual ~VirtualInput() {
        if (registered) {
            Input::unregister_button(this);
        }
    }

protected:
    bool registered = false;

    static bool key_pressed(Key key) { return Input::pressed(key); }
};
}  // namespace auk