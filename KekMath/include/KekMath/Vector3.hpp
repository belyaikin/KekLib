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

        [[nodiscard]]
        constexpr float Dot(const Vector3& other) const
        {
            return this->x * other.x + this->y * other.y + this->z * other.z;
        }
    };

    constexpr Vector3 Vector3::right{1, 0, 0};
    constexpr Vector3 Vector3::left{-1, 0, 0};
    constexpr Vector3 Vector3::up{0, 1, 0};
    constexpr Vector3 Vector3::down{0, -1, 0};
    constexpr Vector3 Vector3::forward{0, 0, 1};
    constexpr Vector3 Vector3::backward{0, 0, -1};
}