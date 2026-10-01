//
// Created by Dmitriy on 01.10.2026.
//

#pragma once
#include <string>
#include <utility>

#include "Object.hpp"

namespace Kek::Graphics::OpenGL
{
    class Shader : public Object
    {
        const std::string path;

    public:
        enum class Type
        {
            Vertex, Fragment
        };

        explicit Shader(const GLuint id, std::string path) : Object(id), path(std::move(path)){}

        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
    };
}
