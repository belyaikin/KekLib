//
// Created by Dmitriy on 01.10.2026.
//

#pragma once

#include <vector>
#include "Buffer.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"

namespace Kek::Graphics::OpenGL
{
    class ObjectFactory
    {
        [[nodiscard]] static std::vector<VertexArray> CreateVertexArrays(size_t n);

        static std::vector<Buffer> CreateBuffers(size_t n);

        static Shader CreateShader(Shader::Type type, const char *source);

        static ShaderProgram CreateShaderProgram(std::vector<const Shader *> shaders);
    };
}
