cmake_minimum_required(VERSION 3.6)
project(KekWorld)

add_library(KekWorld STATIC
        include/KekWorld/Node.hpp
        include/KekWorld/Scene.hpp
        include/KekWorld/Script.hpp
        src/Node.cpp
        include/KekWorld/Transform.hpp
)

target_include_directories(KekWorld PUBLIC include)

target_link_libraries(KekWorld PUBLIC KekMath KekRenderer)