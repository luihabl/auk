#pragma once

#include <auk.h>

#include <vector>

namespace auk {
class Packer {
public:
    struct Node {
        int id;
        int x = 0, y = 0;
        int w = 0, h = 0;
        int page = 0;
        bool packed = false;

        Node(int id, int w, int h);
    };

    Packer(int w, int h);
    void add(int ref, int w, int h);
    bool pack(bool paging = true);
    const std::vector<Node>& get() { return nodes; }
    const std::vector<IVec2>& get_pages() { return pages; }

    void clear();

private:
    BoolVec2 pack_bin_tree();
    std::vector<Node> nodes;
    std::vector<IVec2> pages;
    int max_w, max_h;
    int current_page = 0;
};
}  // namespace auk
