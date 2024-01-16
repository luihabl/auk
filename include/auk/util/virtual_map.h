#pragma once
#include <vector>

namespace auk {

template <typename T>
struct VirtualMap {
    std::vector<T> cells;
    int nx = 0, ny = 0;
    int w = 0, h = 0;

    VirtualMap() = default;
    VirtualMap(int columns, int rows, int cell_width, int cell_height)
        : nx(columns), ny(rows), w(cell_width), h(cell_height) {
        cells.resize((size_t)nx * ny);
    }
};

}  // namespace auk