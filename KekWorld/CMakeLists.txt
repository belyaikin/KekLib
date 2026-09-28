cmake_minimum_required(VERSION 3.6)
project(KekWorld)

add_library(KekWorld STATIC
        include/KekWorld/Nodes/Node.hpp
        src/Node.cpp
        include/KekWorld/Nodes/Renderable/RenderableNode.hpp
        include/KekWorld/Nodes/Renderable/LineNode.hpp
        include/KekWorld/Nodes/Renderable/LineNode.hpp
        include/KekWorld/Scene.hpp
)

target_include_directories(KekWorld PUBLIC include)

target_link_libraries(KekWorld PUBLIC KekMath KekRenderer)