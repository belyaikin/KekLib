//
// Created by Dmitriy on 21.09.2026.
//

#include "KekWorld/Scene.hpp"

namespace Kek::World
{
    void Scene::AddNode(std::unique_ptr<Nodes::Node> node)
    {
        node->SetScene(this);
        nodes.push_back(std::move(node));
    }

    void Scene::DeleteNode(Nodes::Node* node)
    {
        if (!node) return;

        if (Nodes::Node* parent = node->GetParentNode()) {
            parent->RemoveChildNode(node);
        } else {
            std::erase_if(nodes, [node](const std::unique_ptr<Nodes::Node>& n) {
                return n.get() == node;
            });
        }
    }
}
