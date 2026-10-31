cmake_minimum_required(VERSION 3.10)
project(KekGraphics)

add_library(KekGraphics STATIC
        include/KekGraphics/RenderRegistry.hpp
        include/KekGraphics/OpenGL/Objects/Buffer.hpp
        include/KekGraphics/OpenGL/Objects/VertexArray.hpp
        include/KekGraphics/OpenGL/Object.hpp
        include/KekGraphics/OpenGL/Context.hpp
        src/Context.cpp
        include/KekGraphics/Mesh.hpp
        include/KekGraphics/Vertex.hpp
        include/KekGraphics/OpenGL/Objects/Shader.hpp
        include/KekGraphics/OpenGL/Objects/ShaderProgram.hpp
        include/KekGraphics/OpenGL/ObjectFactory.hpp
        src/ObjectFactory.cpp
        src/ShaderProgram.cpp
        include/KekGraphics/MeshBuffers.hpp
        src/MeshBuffers.cpp
        src/Mesh.cpp
        src/MeshDisplay.cpp
        include/KekGraphics/Components/Camera.hpp
        src/Camera.cpp
        include/KekGraphics/Renderer.hpp
        src/Renderer.cpp
        include/KekGraphics/Components/MeshDisplay.hpp
        deps/glad/src/gl.c
        include/KekGraphics/RenderItem.hpp
)

target_include_directories(KekGraphics PUBLIC
        include
        deps/glad/include
)

target_link_libraries(KekGraphics PUBLIC KekMath KekWorld KekAssets)
