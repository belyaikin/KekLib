cmake_minimum_required(VERSION 3.6)
project(KekInput)

add_library(KekInput STATIC
        include/KekInput/Key.hpp
        include/KekInput/Input.hpp
        src/Input.cpp
)

target_include_directories(KekInput PUBLIC include)