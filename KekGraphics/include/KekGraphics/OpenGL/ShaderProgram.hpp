//
// Created by Dmitriy on 01.10.2026.
//

#pragma once
#include <vector>

#include "Object.hpp"
#include "Shader.hpp"

namespace Kek::Graphics::OpenGL
{
    class ShaderProgram : public Object
    {
        const std::vector<const Shader*> shaders;

    public:
        explicit ShaderProgram(const GLuint id, std::vector<const Shader*> shaders) :
            Object(id),
            shaders(std::move(shaders)) {}

        [[nodiscard]] const std::vector<const Shader*>& GetShaders() const { return shaders; }
    };
}
