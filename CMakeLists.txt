cmake_minimum_required(VERSION 3.10)
project(KekLib)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("KekMath")
add_subdirectory("KekInput")
add_subdirectory("KekPlatform")
add_subdirectory("KekAssets")
add_subdirectory("KekGraphics")
add_subdirectory("KekWorld")
add_subdirectory("KekApplication")
add_subdirectory("KekSamples")