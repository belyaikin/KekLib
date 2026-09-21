cmake_minimum_required(VERSION 3.6)
project(KekRenderer)

add_library(KekRenderer STATIC
        src/Renderer.cpp
        include/KekRenderer/Renderer.hpp
        include/KekRenderer/IRenderTarget.hpp)

target_include_directories(KekRenderer PUBLIC include)

target_include_directories(KekRenderer PRIVATE
        deps/glfw/include
        deps/glad/include
)