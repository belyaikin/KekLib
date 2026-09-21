//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include <memory>
#include <vector>

#include "Node.hpp"

namespace Kek::World
{
    class Scene
    {
        std::vector<std::unique_ptr<Node>> nodes;

    public:
        void AddNode(std::unique_ptr<Node> node);
        void DeleteNode(Node *node);

        template <typename T>
        std::vector<T*> GetNodesOfType()
        {
            std::vector<T*> found = {};

            for (const auto& node : this->nodes)
            {
                if (auto child = dynamic_cast<T*>(node.get()))
                    found.push_back(child);
            }

            return found;
        }
    };
}
