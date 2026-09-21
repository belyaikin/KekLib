cmake_minimum_required(VERSION 3.6)
project(KekRenderer)

include_directories("deps/glfw/include")
include_directories("deps/glad/include")

add_library(KekRenderer STATIC
        src/Renderer.cpp
        include/KekRenderer/Renderer.hpp
        include/KekRenderer/IRenderTarget.hpp)

target_include_directories(KekRenderer PUBLIC include)