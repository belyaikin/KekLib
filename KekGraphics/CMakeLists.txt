cmake_minimum_required(VERSION 3.10)
project(KekGraphics)

add_library(KekGraphics STATIC
        include/KekGraphics/IRenderTarget.hpp
        include/KekGraphics/OpenGL/Buffer.hpp
        include/KekGraphics/OpenGL/VertexArray.hpp
        include/KekGraphics/OpenGL/Object.hpp
        include/KekGraphics/OpenGL/Context.hpp
        src/Context.cpp
        include/KekGraphics/Model.hpp
        include/KekGraphics/Vertex.hpp
        include/KekGraphics/OpenGL/Shader.hpp
        include/KekGraphics/OpenGL/ShaderProgram.hpp
        include/KekGraphics/OpenGL/ObjectFactory.hpp
        src/ObjectFactory.cpp
        src/ShaderProgram.cpp
        include/KekGraphics/Mesh.hpp
        src/Mesh.cpp
        src/Model.cpp
        include/KekGraphics/Components/MeshDisplay.hpp
        deps/glad/src/gl.c
)

target_include_directories(KekGraphics PUBLIC
        include
        deps/glad/include
)

target_link_libraries(KekGraphics PUBLIC KekMath KekWorld)