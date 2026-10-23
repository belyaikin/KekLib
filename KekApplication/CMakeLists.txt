cmake_minimum_required(VERSION 3.10)
project(KekApplication)

add_library(KekApplication STATIC
        include/KekApplication/Application.hpp
        src/Application.cpp
        ../KekSamples/00_Smoke/main.cpp)

target_include_directories(KekApplication PUBLIC include)

target_link_libraries(KekApplication PUBLIC KekPlatform KekGraphics KekWorld)