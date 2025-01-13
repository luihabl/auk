cmake_minimum_required(VERSION 3.11)
project(auk)

# C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/external.cmake)

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


