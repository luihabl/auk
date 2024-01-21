#include "auk/util/packer.h"

#define RECT_PACK_H_IMPL
#include <rect_pack.h>

#include <algorithm>
#include <map>

using namespace auk;

Packer::Node::Node(int _id, int _w, int _h) {
    id = _id;
    w = _w;
    h = _h;
}

Packer::Packer(int w, int h) {
    max_w = w;
    max_h = h;
}

bool Packer::pack(bool paging) {
    std::vector<rect_r> rects(nodes.size());

    for (size_t i = 0; i < nodes.size(); i++) {
        rects[i].w = nodes[i].w;
        rects[i].h = nodes[i].h;
        rects[i].id = nodes[i].id;
    }

    bool packed = rect_pack(max_w, max_h, paging, rects.data(), rects.size());

    std::map<int, IVec2> pg;

    for (size_t i = 0; i < nodes.size(); i++) {
        nodes[i].w = rects[i].w;
        nodes[i].h = rects[i].h;
        nodes[i].x = rects[i].info.x;
        nodes[i].y = rects[i].info.y;
        nodes[i].id = rects[i].id;
        nodes[i].packed = rects[i].info.packed;
        nodes[i].page = rects[i].info.page;

        int pgid = nodes[i].page;

        if (pg.count(pgid) == 0) {
            pg[pgid] = IVec2({-1, -1});
        }

        int w = nodes[i].x + nodes[i].w;
        int h = nodes[i].y + nodes[i].h;
        pg[pgid][0] = std::max(pg[pgid][0], w);
        pg[pgid][1] = std::max(pg[pgid][1], h);
    }

    pages.clear();
    pages.reserve(pg.size());
    for (const auto& [idx, sz] : pg) {
        pages.push_back(sz);
    }

    return packed;
}

void Packer::add(int ref, int w, int h) {
    nodes.emplace_back(ref, w, h);
}

void Packer::clear() {
    nodes.clear();
    pages.clear();
}