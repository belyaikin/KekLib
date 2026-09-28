//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include <vector>

namespace Kek::Nodes {
    class Node;
}

namespace Kek::World
{
    class Scene
    {
        std::vector<std::unique_ptr<Nodes::Node>> nodes;

    public:
        void AddNode(std::unique_ptr<Nodes::Node> node);
        void DeleteNode(Nodes::Node *node);

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
