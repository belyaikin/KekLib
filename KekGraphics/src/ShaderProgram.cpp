//
// Created by Dmitriy on 01.10.2026.
//

#include "KekGraphics/OpenGL/ShaderProgram.hpp"

namespace Kek::Graphics::OpenGL
{
    GLint ShaderProgram::GetUniformLocation(const std::string& name) const
    {
        return glGetUniformLocation(id, name.c_str());
    }

    void ShaderProgram::SetBool(const std::string& name, const bool value) const
    {
        glUniform1i(GetUniformLocation(name), value ? 1 : 0);
    }

    void ShaderProgram::SetInt(const std::string& name, const int value) const
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void ShaderProgram::SetFloat(const std::string& name, const float value) const
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void ShaderProgram::SetMatrix4(const std::string& name, const Math::Matrix4& value) const
    {
        // Matrix4 is column-major already, so no transpose is needed.
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value.Data());
    }
}