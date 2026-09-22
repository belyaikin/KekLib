//
// Created by Dmitriy on 22.09.2026.
//

#pragma once
#include "glad/gl.h"

namespace Kek::Renderer::OpenGL
{
    class Object
    {
    protected:
        GLuint id = 0;

    public:
        Object() = default;
        explicit Object(const GLuint id) : id(id) {}

        [[nodiscard]] GLuint GetId() const { return id; }
    };
}
