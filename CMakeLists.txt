cmake_minimum_required(VERSION 3.24)

project(gx)

set(CMAKE_COLOR_DIAGNOSTICS TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(deps)

configure_file(build-info.hpp.in include/build-info.hpp @ONLY)

add_library(gx
    box.cpp
    error.cpp
    id.cpp
    renderer.cpp
    scene.cpp
    sprite.cpp
)

target_include_directories(gx PUBLIC
    include
    "${CMAKE_CURRENT_BINARY_DIR}/include"
)
target_link_libraries(gx PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)

add_subdirectory(example)
