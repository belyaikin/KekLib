//
// Created by Dmitriy on 22.09.2026.
//

#pragma once
#include "Object.hpp"

namespace Kek::Graphics::OpenGL
{
    class Buffer final : public Object
    {
    public:
        enum class Type
        {
            Vertex
        };

        explicit Buffer(const GLuint id) : Object(id) {}

        Buffer(Buffer&&) noexcept = default;

        Buffer& operator=(Buffer&& other) noexcept
        {
            if (this != &other)
            {
                Release();
                Object::operator=(std::move(other));
            }
            return *this;
        }

        ~Buffer() { Release(); }

    private:
        void Release()
        {
            if (id != 0) glDeleteBuffers(1, &id);
            id = 0;
        }
    };
}