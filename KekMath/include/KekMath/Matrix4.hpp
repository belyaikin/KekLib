//
// Created by readmaooo on 20.10.2026.
//

#pragma once

#include <array>
#include <cmath>

#include "Vector3.hpp"

namespace Kek::Math
{
    /// @brief 4x4 matrix stored in column-major order (exactly what OpenGL expects).
    ///
    /// Matrices are applied to column vectors, so `a * b` means "apply b first, then a".
    /// The coordinate system is right-handed, +Y is up, and a camera looks along its local -Z.
    class Matrix4
    {
        std::array<float, 16> m{};

    public:
        /// @brief Creates a matrix filled with zeros. Use Identity() for a "do nothing" matrix.
        constexpr Matrix4() = default;

        [[nodiscard]] static constexpr Matrix4 Identity()
        {
            Matrix4 result;
            result.At(0, 0) = result.At(1, 1) = result.At(2, 2) = result.At(3, 3) = 1.0f;
            return result;
        }

        [[nodiscard]] constexpr float& At(const int row, const int column) { return m[column * 4 + row]; }
        [[nodiscard]] constexpr float At(const int row, const int column) const { return m[column * 4 + row]; }

        /// @brief Pointer to 16 floats in column-major order (for glUniformMatrix4fv).
        [[nodiscard]] const float* Data() const { return m.data(); }

        [[nodiscard]] constexpr Matrix4 operator*(const Matrix4& other) const
        {
            Matrix4 result;

            for (int column = 0; column < 4; ++column)
            {
                for (int row = 0; row < 4; ++row)
                {
                    float sum = 0.0f;
                    for (int k = 0; k < 4; ++k) sum += At(row, k) * other.At(k, column);
                    result.At(row, column) = sum;
                }
            }

            return result;
        }

        [[nodiscard]] constexpr Matrix4 Transposed() const
        {
            Matrix4 result;

            for (int row = 0; row < 4; ++row)
                for (int column = 0; column < 4; ++column)
                    result.At(column, row) = At(row, column);

            return result;
        }

        /// @brief Transforms a point (w = 1) and divides by the resulting w.
        [[nodiscard]] constexpr Vector3 TransformPoint(const Vector3& p) const
        {
            const float x = At(0, 0) * p.X() + At(0, 1) * p.Y() + At(0, 2) * p.Z() + At(0, 3);
            const float y = At(1, 0) * p.X() + At(1, 1) * p.Y() + At(1, 2) * p.Z() + At(1, 3);
            const float z = At(2, 0) * p.X() + At(2, 1) * p.Y() + At(2, 2) * p.Z() + At(2, 3);
            const float w = At(3, 0) * p.X() + At(3, 1) * p.Y() + At(3, 2) * p.Z() + At(3, 3);

            return (w != 0.0f && w != 1.0f) ? Vector3{ x / w, y / w, z / w } : Vector3{ x, y, z };
        }

        [[nodiscard]] static constexpr Matrix4 Translation(const Vector3& by)
        {
            Matrix4 result = Identity();
            result.At(0, 3) = by.X();
            result.At(1, 3) = by.Y();
            result.At(2, 3) = by.Z();
            return result;
        }

        [[nodiscard]] static constexpr Matrix4 Scale(const Vector3& by)
        {
            Matrix4 result = Identity();
            result.At(0, 0) = by.X();
            result.At(1, 1) = by.Y();
            result.At(2, 2) = by.Z();
            return result;
        }

        [[nodiscard]] static Matrix4 RotationX(const float radians)
        {
            const float c = std::cos(radians), s = std::sin(radians);
            Matrix4 result = Identity();
            result.At(1, 1) = c;  result.At(1, 2) = -s;
            result.At(2, 1) = s;  result.At(2, 2) = c;
            return result;
        }

        [[nodiscard]] static Matrix4 RotationY(const float radians)
        {
            const float c = std::cos(radians), s = std::sin(radians);
            Matrix4 result = Identity();
            result.At(0, 0) = c;  result.At(0, 2) = s;
            result.At(2, 0) = -s; result.At(2, 2) = c;
            return result;
        }

        [[nodiscard]] static Matrix4 RotationZ(const float radians)
        {
            const float c = std::cos(radians), s = std::sin(radians);
            Matrix4 result = Identity();
            result.At(0, 0) = c;  result.At(0, 1) = -s;
            result.At(1, 0) = s;  result.At(1, 1) = c;
            return result;
        }

        /// @brief Rotation from Euler angles (radians): X is applied first, then Y, then Z.
        [[nodiscard]] static Matrix4 Rotation(const Vector3& euler)
        {
            return RotationZ(euler.Z()) * RotationY(euler.Y()) * RotationX(euler.X());
        }

        /// @brief OpenGL-style perspective projection (clip space z is in [-1, 1]).
        /// @param verticalFov Vertical field of view in radians.
        /// @param aspectRatio Width divided by height.
        [[nodiscard]] static Matrix4 Perspective(const float verticalFov, const float aspectRatio,
                                                 const float nearPlane, const float farPlane)
        {
            const float f = 1.0f / std::tan(verticalFov * 0.5f);

            Matrix4 result;
            result.At(0, 0) = f / aspectRatio;
            result.At(1, 1) = f;
            result.At(2, 2) = (farPlane + nearPlane) / (nearPlane - farPlane);
            result.At(2, 3) = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
            result.At(3, 2) = -1.0f;
            return result;
        }
    };
}