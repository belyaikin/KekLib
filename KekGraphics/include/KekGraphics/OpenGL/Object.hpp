//
// Created by Dmitriy on 22.09.2026.
//

#pragma once

#include <utility>

#include "glad/gl.h"

namespace Kek::Graphics::OpenGL
{
    /// @brief Base of all OpenGL object wrappers. Move-only: an id has exactly one owner.
    class Object
    {
    protected:
        GLuint id = 0;

        Object() = default;
        explicit Object(const GLuint id) : id(id) {}
        ~Object() = default;

        Object(const Object&) = delete;
        Object& operator=(const Object&) = delete;

        Object(Object&& other) noexcept : id(std::exchange(other.id, 0)) {}

        // Derived classes must release their current id before calling this.
        Object& operator=(Object&& other) noexcept
        {
            if (this != &other) id = std::exchange(other.id, 0);
            return *this;
        }

    public:
        [[nodiscard]] GLuint GetId() const { return id; }
    };
}