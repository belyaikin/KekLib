//
// Created by Dmitriy on 01.10.2026.
//

#pragma once

#include "Object.hpp"

namespace Kek::Graphics::OpenGL
{
    class Shader final : public Object
    {
    public:
        enum class Type
        {
            Vertex, Fragment
        };

        explicit Shader(const GLuint id) : Object(id) {}

        Shader(Shader&&) noexcept = default;

        Shader& operator=(Shader&& other) noexcept
        {
            if (this != &other)
            {
                Release();
                Object::operator=(std::move(other));
            }
            return *this;
        }

        ~Shader() { Release(); }

    private:
        void Release()
        {
            if (id != 0) glDeleteShader(id);
            id = 0;
        }
    };
}