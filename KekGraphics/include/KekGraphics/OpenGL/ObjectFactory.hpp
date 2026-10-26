//
// Created by Dmitriy on 01.10.2026.
//

#pragma once

#include <vector>
#include "Objects/Buffer.hpp"
#include "Objects/Shader.hpp"
#include "Objects/VertexArray.hpp"
#include "Objects/ShaderProgram.hpp"

using namespace Kek::Graphics::OpenGL::Objects;

namespace Kek::Graphics::OpenGL
{
    class ObjectFactory final
    {
    public:
        [[nodiscard]] static std::vector<VertexArray> CreateVertexArrays(size_t n);

        [[nodiscard]] static std::vector<Buffer> CreateBuffers(size_t n);

        [[nodiscard]] static VertexArray CreateVertexArray();

        [[nodiscard]] static Buffer CreateBuffer();

        [[nodiscard]] static Shader CreateShader(Shader::Type type, const char *source);

        [[nodiscard]] static ShaderProgram CreateShaderProgram(std::vector<const Shader *> shaders);
    };
}
