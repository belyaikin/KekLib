//
// Created by Dmitriy on 22.09.2026.
//

#pragma once

namespace Kek::Math
{
    class Vector2
    {
        float x, y;

    public:
        Vector2(const float x, const float y) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const
        {
            return Vector2{ x + other.x, y + other.y };
        }

        [[nodiscard]]
        float Dot(const Vector2& other) const
        {
            return this->x * other.x + this->y * other.y;
        }
    };
}