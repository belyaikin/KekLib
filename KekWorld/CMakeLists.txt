cmake_minimum_required(VERSION 3.6)
project(KekWorld)

add_library(KekWorld STATIC
        src/Scene.cpp
        include/KekWorld/Scene.h
        include/KekWorld/Node.h)

target_include_directories(KekWorld PUBLIC include)