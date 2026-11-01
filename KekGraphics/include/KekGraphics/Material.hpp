//
// Created by vladk on 01.11.2026.
//

#pragma once
#include <memory>
#include <string>

#include "OpenGL/Objects/ShaderProgram.hpp"
#include "OpenGL/Objects/Texture.hpp"

namespace Kek::Graphics
{
    struct MaterialSettings final
    {
        std::string vertexShader = "shaders/phong.vert";
        std::string fragmentShader = "shaders/phong.frag";
        std::string texture; // asset path, empty = no texture
    };

    class Material final
    {
        std::shared_ptr<OpenGL::Objects::ShaderProgram> program;
        std::unique_ptr<OpenGL::Objects::Texture> texture;

    public:
        explicit Material(const MaterialSettings& settings);
        [[nodiscard]] const OpenGL::Objects::ShaderProgram& GetProgram()

        const
        {
            return *program;
        }
        /// @brief Binds the texture and sets the sampler uniforms. The program must be in use.
        void Apply() const;
    };
}
