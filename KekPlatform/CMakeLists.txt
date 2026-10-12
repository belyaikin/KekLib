cmake_minimum_required(VERSION 3.10)
project(KekPlatform)

add_library(KekPlatform STATIC include/KekPlatform/Window.hpp
src/Window.cpp)

target_include_directories(KekPlatform PUBLIC include)

target_include_directories(KekPlatform PRIVATE
        deps/glfw/include
)