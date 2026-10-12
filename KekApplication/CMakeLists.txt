cmake_minimum_required(VERSION 3.10)
project(KekApplication)

add_library(KekApplication STATIC
        include/KekApplication/Application.hpp
        src/Application.cpp)

target_include_directories(KekApplication PUBLIC include)