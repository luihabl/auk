# auk 🐧
A minimal C++ toolset for 2D games using SDL2 + OpenGL. 


## Usage example

The snippet bellow shows how to draw a sprite to the screen at a position `{50.0f, 150.0f}`:
```cpp
#include <auk.h>

using namespace auk;

int main(int argc, char *argv[]) {

    int screen_w = 512, screen_h = 256;
    
    SDL_Window * window = Window::init("Window Title", screen_w, screen_h);
    Mat4x4 window_projection = LinAlg::ortho(0, (float) screen_w, (float) screen_h, 0, -1, 1);
    
    BatchRenderer renderer;
    renderer.setup();

    Shader shader = Shader::default_sprite_shaders();
    shader.use().set_mat4x4("projection", window_projection); 

    Texture sprite_tex = Texture::from_file("sprite.png");

    SDL_Event event; 
    bool quit = false;
    while(!quit) {
        while (SDL_PollEvent(&event) != 0) if (event.type == SDL_QUIT) quit = true;

        Graphics::viewport(screen_w, screen_h);
        Graphics::clear(Color::black);

        renderer.set_texture(&sprite_tex);
        renderer.draw_tex({50.0f, 150.0f});

        renderer.render();
        SDL_GL_SwapWindow(window);
    }
    
    return 0;
}

```

## Building

It requires CMake 3.14+ to build. This is a minimal example of a `CMakeLists.txt` file to build something:

```cmake
cmake_minimum_required(VERSION 3.11)
project(example)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(path/to/auk)

add_executable(example main.cpp)

target_link_libraries(example auk)
```

CMake will download automatically SDL2 and it will be built and linked statically to the application.
