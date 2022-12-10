cmake_minimum_required(VERSION 3.23)
project(ProjectX)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        include
)

add_executable(
        rasterize
        src/rasterize.cpp
        src/stbi.c
)
add_executable(
        ray_trace
        src/ray_trace.cpp
        src/stbi.c
)
add_executable(
        matrix_test
        src/math_test.cpp
)
