cmake_minimum_required(VERSION 3.6)
project(KekRenderer)

add_subdirectory("deps/glfw")
include_directories("deps/glad/include")

add_library(KekRenderer STATIC
        src/Renderer.cpp
        include/KekRenderer/Renderer.h)

target_include_directories(KekRenderer PUBLIC include)