cmake_minimum_required(VERSION 3.6)
project(KekMath)

add_library(KekMath INTERFACE
        include/KekMath/Vector2.hpp
        include/KekMath/Vector3.hpp
)

target_include_directories(KekMath INTERFACE include)