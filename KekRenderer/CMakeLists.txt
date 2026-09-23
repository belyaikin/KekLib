cmake_minimum_required(VERSION 3.6)
project(KekRenderer)

add_library(KekRenderer STATIC
        include/KekRenderer/IRenderTarget.hpp
        include/KekRenderer/OpenGL/Buffer.hpp
        include/KekRenderer/OpenGL/VertexArray.hpp
        include/KekRenderer/OpenGL/Object.hpp
        include/KekRenderer/OpenGL/Context.hpp
        src/Context.cpp)

target_include_directories(KekRenderer PUBLIC include)

target_include_directories(KekRenderer PRIVATE
        deps/glfw/include
        deps/glad/include
)