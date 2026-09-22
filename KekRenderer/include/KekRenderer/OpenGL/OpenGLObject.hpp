//
// Created by Dmitriy on 22.09.2026.
//

#pragma once
#include "glad/gl.h"

namespace Kek::Renderer::OpenGL
{
    class OpenGLObject
    {
    protected:
        GLuint id = 0;

    public:
        OpenGLObject() = default;
        virtual ~OpenGLObject() = default;

        OpenGLObject(const OpenGLObject&) = delete;
        OpenGLObject& operator=(const OpenGLObject&) = delete;

        OpenGLObject(OpenGLObject&& other) noexcept : id(other.id)
        {
            other.id = 0;
        }
        OpenGLObject& operator=(OpenGLObject&& other) noexcept
        {
            if (this != &other) {
                id = other.id;
                other.id = 0;
            }
            return *this;
        }

        [[nodiscard]] GLuint GetId() const { return id; }
    };
}
