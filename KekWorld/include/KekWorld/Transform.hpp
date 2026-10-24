//
// Created by Dmitriy on 29.09.2026.
//

#pragma once

#include "Component.hpp"
#include "KekMath/Matrix4.hpp"
#include "KekMath/Vector3.hpp"

namespace Kek::World
{
    class Transform final : public Component
    {
        Math::Vector3 position;
        Math::Vector3 rotation{0, 0, 0}; // Euler angles in radians (X, Y, Z)
        Math::Vector3 scale{1, 1, 1};

    public:
        explicit Transform(const Math::Vector3 &position) : position(position) {}

        [[nodiscard]] Math::Vector3 GetPosition() const { return this->position; }
        void SetPosition(const Math::Vector3 &value) { this->position = value; }

        void Move(const Math::Vector3 &by)
        {
            position = position + by;
        }

        /// @brief Euler angles in radians. Order: X is applied first, then Y, then Z.
        [[nodiscard]] Math::Vector3 GetRotation() const { return this->rotation; }
        void SetRotation(const Math::Vector3 &value) { this->rotation = value; }

        void Rotate(const Math::Vector3 &by)
        {
            rotation = rotation + by;
        }

        [[nodiscard]] Math::Vector3 GetScale() const { return this->scale; }
        void SetScale(const Math::Vector3 &value) { this->scale = value; }

        /// @brief Local-to-parent matrix: translate * rotate * scale.
        [[nodiscard]] Math::Matrix4 GetLocalMatrix() const
        {
            return Math::Matrix4::Translation(position)
                 * Math::Matrix4::Rotation(rotation)
                 * Math::Matrix4::Scale(scale);
        }
    };
}