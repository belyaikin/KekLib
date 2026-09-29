//
// Created by Dmitriy on 29.09.2026.
//

#pragma once
#include "Script.hpp"
#include "KekMath/Vector3.hpp"

namespace Kek::World
{
    class Transform final : public Script
    {
        Math::Vector3 position;

    public:
        explicit Transform(const Math::Vector3 &position) : position(position) {}

        [[nodiscard]] Math::Vector3 GetPosition() const { return this->position; }
    };
}
