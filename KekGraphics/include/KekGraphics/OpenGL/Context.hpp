//
// Created by Dmitriy on 22.09.2026.
//

#pragma once

#include <cstddef>

#include "Buffer.hpp"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"

namespace Kek::Graphics::OpenGL
{
    class Context final
    {
    public:
        using LoaderFunc = void* (*)(const char*);
        [[nodiscard]] bool Initialize(LoaderFunc loader) const;

        // Everything below only talks to the current OpenGL context,
        // so it is static and can be called from anywhere after Initialize().

        static void BindVertexArray(const VertexArray& vertexArray);

        static void UnbindVertexArray();

        static void BindBuffer(const Buffer& buffer, Buffer::Type type);

        /// @brief Uploads data into the buffer currently bound to the given target.
        static void BufferData(Buffer::Type type, const void* data, std::size_t bytes);

        /// @brief Describes a float attribute of the currently bound VAO/VBO and enables it.
        static void EnableVertexAttribute(GLuint index, GLint components, std::size_t stride, std::size_t offset);

        static void DrawArrays(GLsizei vertexCount);

        static void DrawElements(GLsizei indexCount);

        [[nodiscard]] static GLenum GetError();

        static void SetViewport(int x, int y, int width, int height);

        static void UseProgram(const ShaderProgram& program);

        static void SetClearColor(float r, float g, float b, float a);

        static void EnableDepthTest();
       /// @brief Clears the color and depth buffers of the current framebuffer.
        static void Clear();
    };
}