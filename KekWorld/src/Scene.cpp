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

    void Scene::DeleteNode(Node* node)
    {
        if (!node) return;

        if (Node* parent = node->GetParentNode()) {
            parent->RemoveChildNode(node);
        } else {
            std::erase_if(nodes, [node](const std::unique_ptr<Node>& n) {
                return n.get() == node;
            });
        }
    }
}
