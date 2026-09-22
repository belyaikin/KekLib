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
        Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3& other) const
        {
            return Vector3{ x + other.x, y + other.y, z + other.z };
        }

        [[nodiscard]]
        float Dot(const Vector3& other) const
        {
            return this->x * other.x + this->y * other.y + this->z * other.z;
        }
    };
}