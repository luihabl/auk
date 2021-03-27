cmake_minimum_required(VERSION 3.11)
project(tinysdl)

# C++ version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# adding files
add_library(tinysdl 

src/core/window.cpp
src/core/log.cpp
src/core/file.cpp

src/graphics/graphics.cpp
src/graphics/shader.cpp
src/graphics/texture.cpp
src/graphics/renderer.cpp

src/ext/glad.cpp
) 

target_include_directories(tinysdl
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/tinysdl/ext>
)

if (NOT DEFINED SDL2_INCLUDE_DIRS OR NOT DEFINED SDL2_LIBRARIES)
    find_package(SDL2 REQUIRED)
endif()

target_include_directories(tinysdl PUBLIC "$<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>")
target_link_libraries(tinysdl PUBLIC ${SDL2_LIBRARIES})


