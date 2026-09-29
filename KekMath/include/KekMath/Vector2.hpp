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
        static const Vector2 right;
        static const Vector2 left;
        static const Vector2 up;
        static const Vector2 down;

        constexpr Vector2(const float x, const float y) : x(x), y(y) {}

        constexpr Vector2 operator+(const Vector2& other) const
        {
            return Vector2{ x + other.x, y + other.y };
        }

        constexpr Vector2& operator+=(const Vector2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        [[nodiscard]]
        constexpr float Dot(const Vector2& other) const
        {
            return this->x * other.x + this->y * other.y;
        }
    };

    constexpr Vector2 Vector2::right{1, 0};
    constexpr Vector2 Vector2::left{-1, 0};
    constexpr Vector2 Vector2::up{0, 1};
    constexpr Vector2 Vector2::down{0, -1};
}