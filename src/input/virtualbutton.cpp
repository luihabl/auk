#include <auk.h>

using namespace auk;

VirtualButton & VirtualButton::set_repeat(float first_delay, float multi_delay) {
    can_repeat = true;
    repeat_first_delay_ms = first_delay;
    repeat_multi_delay_ms = multi_delay;
    return *this;
}

VirtualButton & VirtualButton::add(Key key) {
    nodes.push_back(key);
    return *this;
}

void VirtualButton::update() {

    bool last_press = btn_pressed;
    btn_pressed = false;
    for(auto & node : nodes) {
        if(node.check()) {
            btn_pressed = true;
            break;
        }
    }

    if (!btn_pressed) {
        repeating = false;
        first_repeat = true;
        repeat_timer.reset();
    }
    else if(can_repeat) {

        if(repeat_timer.is_paused) {
            repeat_timer.start();
        }
        else {

            repeating = false;

            if((repeat_timer.get_time_ms() >= repeat_first_delay_ms) && first_repeat) {
                repeat_timer.reset();
                repeat_timer.start();
                repeating = true;
                first_repeat = false;
            }

            if ((repeat_timer.get_time_ms() >= repeat_multi_delay_ms) && !first_repeat) {
                repeat_timer.reset();
                repeat_timer.start();
                repeating = true;   
            }
        }
    }

    btn_just_pressed = false;
    if ((btn_pressed && !last_press) || repeating) {
        btn_just_pressed = true;
    }
    
    btn_released = false;
    if (last_press && !btn_pressed) {
        btn_released = true;
    }
    
}

bool VirtualButton::KeyNode::check() {
    return key_pressed(key);
}