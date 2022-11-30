#pragma once

#include <auk.h>
#include <vector>

namespace auk
{
    class Packer
    {

    public:
        struct Node
        {
            int id;
            int x = 0, y = 0;
            int w = 0, h = 0;
            int page = 0;
            bool packed = false;

            Node(int id, int w, int h);
        };

        Packer(const IVec2& max_size);
        void add(int ref, int w, int h);
        bool pack(bool paging = true);
        void clear();
    
    private:
        BoolVec2 pack_bin_tree();
        std::vector<Node> nodes;
        std::vector<IVec2> pages;
        IVec2 max_page_size = {0, 0};
        int current_page = 0;

    };
}
