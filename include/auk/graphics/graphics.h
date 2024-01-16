#pragma once

typedef void* (*GraphicsLoaderFunction)(const char* name);

namespace auk {
struct Color;

namespace Graphics {
void load_functions(GraphicsLoaderFunction loader_func);
void setup_debug();

void clear(const Color& c);
void set_blend_mode();
void viewport(int w, int h);
void viewport(int x, int y, int w, int h);
}  // namespace Graphics
}  // namespace auk