//
// Created by Dmitriy on 21.09.2026.
//

#include "../include/KekWorld/Node.hpp"

#include <memory>

namespace Kek::World
{
    void Node::AddChildNode(std::unique_ptr<Node> node)
    {
        node->parentNode = this;
        Node* raw = node.get();
        childNodes.push_back(std::move(node));
        raw->Start();
    }

    void Node::RemoveChildNode(Node *node)
    {
        std::erase_if(childNodes, [node](const std::unique_ptr<Node>& n) {
            return n.get() == node;
        });
    }

    void Node::Start() {
        for (const auto& child : this->childNodes)
        {
            child->Start();
        }

        for (const auto& script : this->components)
        {
            script->Start();
        }
    }

    void Node::Tick()
    {
        for (const auto& child : this->childNodes)
        {
            child->Tick();
        }

        for (const auto& script : this->components)
        {
            script->Tick();
        }
    }
}
