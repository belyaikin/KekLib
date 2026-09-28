//
// Created by Dmitriy on 28.09.2026.
//

#pragma once
#include <memory>

#include "Nodes/Node.hpp"

namespace Kek::World
{
    class Scene
    {
    private:
        std::unique_ptr<Nodes::Node> masterNode;

    public:
        explicit Scene(Nodes::Node masterNode) : masterNode(masterNode) {}
    };
}
