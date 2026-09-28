cmake_minimum_required(VERSION 3.6)
project(KekNodes)

add_library(KekNodes STATIC
        include/KekNodes/Node.hpp
        src/Node.cpp
        include/KekNodes/RenderableNode.hpp
)

target_include_directories(KekNodes PUBLIC include)

target_link_libraries(KekNodes PUBLIC KekMath KekRenderer)