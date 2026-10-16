//
// Created by Dmitriy on 28.09.2026.
//

#pragma once
#include <memory>

#include "Node.hpp"

namespace Kek::World
{
    class Scene
    {
        std::unique_ptr<Node> masterNode;

        std::vector<Node*> pendingRemoval;

    public:
        explicit Scene(std::unique_ptr<Node> masterNode) : masterNode(std::move(masterNode)) {}

        [[nodiscard]] Node *GetMasterNode() const { return masterNode.get(); }

        void Start();
        void Tick();
    };
}
