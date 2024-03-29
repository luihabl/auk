cmake_minimum_required(VERSION 3.11)
project(auk)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG origin/release-2.26.x # grab latest stable release
    GIT_PROGRESS TRUE
)
set(SDL_TEST_ENABLED_BY_DEFAULT OFF CACHE BOOL "Build the SDL2_test library by default" FORCE)
set(SDL_TEST OFF CACHE BOOL "Build the SDL2_test library" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Build the SDL2 shared library" FORCE)
FetchContent_MakeAvailable(sdl2)

FetchContent_Declare(
    rect_pack
    GIT_REPOSITORY https://github.com/luihabl/rect_pack.h.git
    GIT_PROGRESS TRUE
    GIT_TAG main
)
FetchContent_MakeAvailable(rect_pack)

# adding files
add_library(auk 

src/platform/window.cpp
src/platform/log.cpp
src/platform/file.cpp
src/platform/time.cpp

src/graphics/graphics.cpp
src/graphics/shader.cpp
src/graphics/texture.cpp
src/graphics/renderer.cpp
src/graphics/target.cpp
src/graphics/color.cpp

src/numerics/linalg.cpp
src/numerics/mathf.cpp

src/input/input.cpp
src/input/virtualaxis.cpp
src/input/virtualbutton.cpp

src/util/packer.cpp

src/external/glad.cpp
) 

target_include_directories(auk
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/auk/external>
)

target_link_libraries(auk PUBLIC SDL2main SDL2-static rect_pack)


