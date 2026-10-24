cmake_minimum_required(VERSION 3.10)
project(KekPlatform)

add_subdirectory(deps/glfw)

add_library(KekPlatform STATIC include/KekPlatform/Window.hpp
src/Window.cpp)

target_include_directories(KekPlatform PUBLIC include)

target_link_libraries(KekPlatform PRIVATE glfw)

target_compile_definitions(KekPlatform PUBLIC GLFW_INCLUDE_NONE)

