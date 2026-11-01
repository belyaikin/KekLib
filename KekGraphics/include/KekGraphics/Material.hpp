//
// Created by vladk on 01.11.2026.
//

#pragma once
#include <memory>
#include <string>
#include "OpenGL/Objects/ShaderProgram.hpp"

namespace Kek::Graphics
{
    /// @brief What a material is made of. Plain data, no OpenGL.
    struct MaterialSettings final
    {
        std::string vertexShader = "shaders/phong.vert";
        std::string fragmentShader = "shaders/phong.frag";
    };

    /// @brief GPU-side material. Create it only while the OpenGL context is current.
    class Material final
    {
        std::shared_ptr<OpenGL::Objects::ShaderProgram> program;

    public:
        explicit Material(const MaterialSettings& settings);
        
        [[nodiscard]] const OpenGL::Objects::ShaderProgram& GetProgram() const
        {
            return *program;
        }
    };
}