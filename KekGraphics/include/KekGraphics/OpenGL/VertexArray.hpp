//
// Created by Dmitriy on 22.09.2026.
//

#pragma once
#include "Object.hpp"

namespace Kek::Graphics::OpenGL
{
    class VertexArray final : public Object
    {
    public:
        explicit VertexArray(const GLuint id) : Object(id) {}

        VertexArray(VertexArray&&) noexcept = default;

        VertexArray& operator=(VertexArray&& other) noexcept
        {
            if (this != &other)
            {
                Release();
                Object::operator=(std::move(other));
            }
            return *this;
        }

        ~VertexArray() { Release(); }

    private:
        void Release()
        {
            if (id != 0) glDeleteVertexArrays(1, &id);
            id = 0;
        }
    };
}