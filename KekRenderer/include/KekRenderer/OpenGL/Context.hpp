//
// Created by Dmitriy on 22.09.2026.
//

#pragma once

#include <vector>

#include "Buffer.hpp"
#include "VertexArray.hpp"

namespace Kek::Renderer::OpenGL
{
    class Context
    {
        [[nodiscard]] std::vector<VertexArray> CreateVertexArrays(int n);
        void BindVertexArray(const VertexArray& vertexArray);
        void DestroyVertexArrays(const std::vector<VertexArray> &vertexArrays);

        std::vector<Buffer> CreateBuffers(size_t n);
    };
}
