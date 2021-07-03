# TinySDL
A minimal C++ toolset for 2D games using SDL and OpenGL. 


## Usage example

The snippet bellow shows how to draw a sprite to the screen at a position `{50.0f, 150.0f}`:
```cpp
#include <tinysdl.h>

using namespace TinySDL;

int main(int argc, char *argv[]) {

    int screen_w = 512, screen_h = 256;
    
    SDL_Window * window = Window::init("Window Title", screen_w, screen_h);
    Mat4x4 window_projection = LinAlg::ortho(0, (float) screen_w, (float) screen_h, 0, -1, 1);
    
    BatchRenderer renderer;

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

It requires SDL 2 and CMake 3.11+ to build. This is a minimal example of a `CMakeLists.txt` file to build something:

```cmake
cmake_minimum_required(VERSION 3.11)
project(example)

# C++ version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(path/to/tinysdl)

add_executable(example main.cpp)

target_link_libraries(example tinysdl)
```

Note that CMake needs to find SDL, so if it can't find it on its default search paths you need to provide a path using the `SDL2_DIR` variable. In terminal/powershell this can be done once as `cmake -DSDL2_DIR=path/to/sdl ..` (assuming you are building from a `build` directory). See [this blog post](https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/) for further information. 

If you are on Windows the `SDL2.dll` needs to be on the system's PATH or it needs to be copied to the same directory of the game's executable. This can be done manually or you can use CMake to copy it automatically using something like:

```cmake
if (EXISTS ${SDL2_DLL})
    add_custom_command(
        TARGET example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_DLL} $<TARGET_FILE_DIR:example>)
endif()
```

In this case you need to provide the dll path using the `SDL2_DLL` variable.  
