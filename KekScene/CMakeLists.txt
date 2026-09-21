cmake_minimum_required(VERSION 3.6)
project(KekScene)

add_library(KekScene STATIC
        src/Scene.cpp
        include/KekScene/Scene.h
        include/KekScene/Node.h)

target_include_directories(KekScene PUBLIC include)