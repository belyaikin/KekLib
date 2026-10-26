//
// Created by Dmitriy on 01.10.2026.
//

#pragma once
#include <string>
#include "../Object.hpp"
#include "KekMath/Matrix4.hpp"

namespace Kek::Graphics::OpenGL::Objects
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
        // Uniform setters: the program must be in use (see Context::UseProgram).
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetMatrix4(const std::string& name, const Math::Matrix4& value) const;

    private:
        [[nodiscard]] GLint GetUniformLocation(const std::string& name) const;
        void Release()
        {
            if (id != 0) glDeleteProgram(id);
            id = 0;
        }
    };
}