//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

namespace Kek::Math
{
    class Vector3
    {
        float x, y, z;

    public:
        static const Vector3 right;
        static const Vector3 left;
        static const Vector3 up;
        static const Vector3 down;
        static const Vector3 forward;
        static const Vector3 backward;

        constexpr Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

        [[nodiscard]] constexpr float X() const { return x; }
        [[nodiscard]] constexpr float Y() const { return y; }
        [[nodiscard]] constexpr float Z() const { return z; }

        constexpr Vector3 operator+(const Vector3& other) const
        {
            return Vector3{ x + other.x, y + other.y, z + other.z };
        }

        constexpr Vector3& operator+=(const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr Vector3 operator*(const float by) const
        {
            return Vector3{ x * by, y * by, z * by };
        }

        constexpr Vector3 operator*(const Vector3 &by) const
        {
            return Vector3{ x * by.x, y * by.y, z * by.z };
        }

        friend constexpr Vector3 operator*(const float by, const Vector3& v)
        {
            return v * by;
        }

        constexpr Vector3& operator*=(const float by)
        {
            x *= by;
            y *= by;
            z *= by;
            return *this;
        }

        constexpr Vector3& operator*=(const Vector3 &by)
        {
            x *= by.x;
            y *= by.y;
            z *= by.z;
            return *this;
        }
    };

    constexpr Vector3 Vector3::right{1, 0, 0};
    constexpr Vector3 Vector3::left{-1, 0, 0};
    constexpr Vector3 Vector3::up{0, 1, 0};
    constexpr Vector3 Vector3::down{0, -1, 0};
    constexpr Vector3 Vector3::forward{0, 0, 1};
    constexpr Vector3 Vector3::backward{0, 0, -1};
}