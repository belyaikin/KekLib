//
// Created by vladk on 01.11.2026.
//

#pragma once
#include "../Object.hpp"

namespace Kek::Graphics::OpenGL::Objects
{
    class Texture final : public Object
    {
    public:
        explicit Texture(const GLuint id) : Object(id) {}

        Texture(Texture&&) noexcept = default;

        Texture& operator=(Texture&& other) noexcept
        {
            if (this != &other)
            {
                Release();
                Object::operator=(std::move(other));
            }
            return *this;
        }

        ~Texture() { Release(); }

    private:
        void Release()
        {
            if (id != 0) glDeleteTextures(1, &id);
            id = 0;
        }
    };
}