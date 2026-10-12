//
// Created by Dmitriy on 22.09.2026.
//

#pragma once

#include <vector>

#include "Buffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace Kek::Graphics::OpenGL
{
    class Context
    {
    public:
        using LoaderFunc = void* (*)(const char*);
        [[nodiscard]] bool Initialize(LoaderFunc loader) const;

        void BindVertexArray(const VertexArray& vertexArray);
        void DestroyVertexArrays(const std::vector<VertexArray> &vertexArrays);

        void BindBuffer(const Buffer& buffer, Buffer::Type type);
    };
}
