//
// Created by Dmitriy on 21.09.2026.
//

#include "KekWorld/Scene.hpp"

namespace Kek::World
{
    void Scene::AddNode(std::unique_ptr<Node> node)
    {
        node->SetScene(this);
        nodes.push_back(std::move(node));
    }

    void Scene::DeleteNode(Node *node)
    {
        if (!node) return;

        for (auto& childNode : node->GetChildNodes()) {
            DeleteNode(childNode.get());
        }

        node->SetScene(nullptr);

        std::erase_if(this->nodes, [&node](const std::unique_ptr<Node>& managedNode) {
            return managedNode.get() == node;
        });
    }
}
