//
// Created by vladk on 01.11.2026.
//

#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "OpenGL/Objects/ShaderProgram.hpp"

namespace Kek::Graphics
{
    /// @brief Compiles shader programs from asset files and shares them between objects.
    /// Use it only while the OpenGL context is current.
    class ShaderLibrary final
    {
        inline static std::unordered_map<std::string, std::weak_ptr<OpenGL::Objects::ShaderProgram>> cache;
    public:
        [[nodiscard]] static std::shared_ptr<OpenGL::Objects::ShaderProgram> Load(const std::string& vertexPath, const std::string& fragmentPath);
    };
}
