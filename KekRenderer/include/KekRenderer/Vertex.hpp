//
// Created by Dmitriy on 29.09.2026.
//

#pragma once
#include "KekMath/Vector3.hpp"

namespace Kek::Renderer
{
    class Vertex
    {
        Math::Vector3 position;

    public:
        explicit Vertex(const Math::Vector3 position) : position(position) {}
    };
}
