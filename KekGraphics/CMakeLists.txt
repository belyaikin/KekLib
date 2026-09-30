cmake_minimum_required(VERSION 3.6)
project(KekGraphics)

add_library(KekGraphics STATIC
        include/KekGraphics/IRenderTarget.hpp
        include/KekGraphics/OpenGL/Buffer.hpp
        include/KekGraphics/OpenGL/VertexArray.hpp
        include/KekGraphics/OpenGL/Object.hpp
        include/KekGraphics/OpenGL/Context.hpp
        src/Context.cpp
        include/KekGraphics/Model.hpp
        include/KekGraphics/Vertex.hpp)

target_include_directories(KekGraphics PUBLIC include)

target_include_directories(KekGraphics PRIVATE
        deps/glfw/include
        deps/glad/include
)

target_link_libraries(KekGraphics PUBLIC KekMath)