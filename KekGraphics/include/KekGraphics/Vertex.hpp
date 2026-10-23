//
// Created by Dmitriy on 29.09.2026.
//

#pragma once
#include "KekMath/Vector3.hpp"

namespace Kek::Graphics
{
    class Vertex
    {
        Math::Vector3 position;
        Math::Vector3 normal;

    public:
        Vertex(const Math::Vector3 position, const Math::Vector3 normal)
            : position(position), normal(normal) {}

        [[nodiscard]] const Math::Vector3& GetPosition() const { return position; }
        [[nodiscard]] const Math::Vector3& GetNormal() const { return normal; }
    };
}
