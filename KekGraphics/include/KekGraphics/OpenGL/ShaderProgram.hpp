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
        const std::vector<Shader> shaders;
        
    public:
        explicit ShaderProgram(std::vector<Shader> shaders) : shaders(std::move(shaders)) {}
    };
}
