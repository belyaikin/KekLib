//
// Created by Dmitriy on 22.09.2026.
//

#pragma once
#include "Object.hpp"

namespace Kek::Renderer::OpenGL
{
    class Buffer : public Object
    {
    public:
        enum class Type
        {
            Vertex
        };

        explicit Buffer(const GLuint id) : Object(id) {}
    };
}
