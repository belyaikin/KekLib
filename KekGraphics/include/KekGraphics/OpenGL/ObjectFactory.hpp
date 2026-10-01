//
// Created by Dmitriy on 01.10.2026.
//

#pragma once

#include <vector>
#include "Buffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace Kek::Graphics::OpenGL
{
    class ObjectFactory
    {
        [[nodiscard]] std::vector<VertexArray> CreateVertexArrays(size_t n);

        std::vector<Buffer> CreateBuffers(size_t n);

        Shader CreateShader(Shader::Type type, const char *source);
    };
}
