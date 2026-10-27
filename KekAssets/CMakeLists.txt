cmake_minimum_required(VERSION 3.10)
project(KekAssets)

add_library(KekAssets STATIC
        include/KekAssets/Assets.hpp
        src/Assets.cpp
)

target_include_directories(KekAssets PUBLIC include PRIVATE deps/stb)
target_compile_definitions(KekAssets PRIVATE KEK_DEFAULT_ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")