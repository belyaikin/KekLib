//
// Created by Dmitriy on 29.09.2026.
//

#pragma once

#include "Component.hpp"
#include "KekMath/Vector3.hpp"

namespace Kek::World
{
    class Transform final : public Component
    {
        Math::Vector3 position;

    public:
        explicit Transform(const Math::Vector3 &position) : position(position) {}

        [[nodiscard]] Math::Vector3 GetPosition() const { return this->position; }

        void Move(const Math::Vector3 &by)
        {
            position = position + by;
        }
    };
}
