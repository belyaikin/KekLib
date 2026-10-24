//
// Created by Dmitriy on 01.10.2026.
//

#pragma once

#include "Object.hpp"

namespace Kek::Graphics::OpenGL
{
    class ShaderProgram final : public Object
    {
    public:
        explicit ShaderProgram(const GLuint id) : Object(id) {}

        ShaderProgram(ShaderProgram&&) noexcept = default;

        ShaderProgram& operator=(ShaderProgram&& other) noexcept
        {
            if (this != &other)
            {
                Release();
                Object::operator=(std::move(other));
            }
            return *this;
        }

        ~ShaderProgram() { Release(); }

    private:
        void Release()
        {
            if (id != 0) glDeleteProgram(id);
            id = 0;
        }
    };
}