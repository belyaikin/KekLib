//
// Created by Dmitriy on 28.09.2026.
//

#pragma once

#include "RenderableNode.hpp"
#include <KekMath/Vector3.hpp>

namespace Kek::Nodes::Renderable
{
    class LineNode : RenderableNode
    {
        std::vector<Math::Vector3> points;

    public:
        explicit LineNode(const std::vector<Math::Vector3>& points) : points(points) {}
    };
}
