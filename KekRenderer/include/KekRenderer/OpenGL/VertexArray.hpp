//
// Created by Dmitriy on 22.09.2026.
//

#pragma once
#include "Object.hpp"

namespace Kek::Renderer::OpenGL
{
    class VertexArray : public Object
    {
    public:
        explicit VertexArray(const GLuint id) : Object(id) {}
    };
}
