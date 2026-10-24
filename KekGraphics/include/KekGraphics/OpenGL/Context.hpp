//
// Created by Dmitriy on 22.09.2026.
//

#pragma once

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

        static void BindBuffer(const Buffer& buffer, Buffer::Type type);

        static void UseProgram(const ShaderProgram& program);

        static void SetClearColor(float r, float g, float b, float a);

        /// @brief Clears the color buffer of the current framebuffer.
        static void Clear();
    };
}