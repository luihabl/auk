#include <auk.h>

using namespace auk;

VirtualAxis & VirtualAxis::add(Key negative, Key positive) {
    nodes.push_back({negative, positive});
    return *this;
}

void VirtualAxis::update() {
    
    for(auto & node : nodes)
        node.update();

    val = 0;
    for(auto & node : nodes) {
        if(node.val != 0) {
            val = node.val;
            break;
        }
    }
}

void VirtualAxis::KeyAxisNode::update() {

    if (key_pressed(positive)) {

        if (key_pressed(negative)) {
            val = 0.0f;
        }
        else {
            val = 1.0f;
        }
    }
    else if (key_pressed(negative)) {
        val = -1.0f;
    }
    else {
        val = 0.0f;
    }

}