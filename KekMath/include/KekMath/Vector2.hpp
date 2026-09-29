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

        [[nodiscard]] constexpr float GetX() const { return x; }
        [[nodiscard]] constexpr float GetY() const { return y; }

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

        constexpr Vector2 operator*(const float by) const
        {
            return Vector2{ x * by, y * by };
        }

        constexpr Vector2 operator*(const Vector2 &by) const
        {
            return Vector2{ x * by.x, y * by.y };
        }

        friend constexpr Vector2 operator*(const float by, const Vector2& v)
        {
            return v * by;
        }

        constexpr Vector2& operator*=(const float by)
        {
            x *= by;
            y *= by;
            return *this;
        }

        constexpr Vector2& operator*=(const Vector2 &by)
        {
            x *= by.x;
            y *= by.y;
            return *this;
        }
    };

    constexpr Vector2 Vector2::right{1, 0};
    constexpr Vector2 Vector2::left{-1, 0};
    constexpr Vector2 Vector2::up{0, 1};
    constexpr Vector2 Vector2::down{0, -1};
}