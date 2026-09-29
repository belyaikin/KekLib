//
// Created by Dmitriy on 21.09.2026.
//

#include "../include/KekWorld/Node.hpp"

namespace Kek::World
{
    void Node::AddChildNode(std::unique_ptr<Node> node)
    {
        node->parentNode = this;
        childNodes.push_back(std::move(node));
    }

    void Node::RemoveChildNode(Node *node)
    {
        std::erase_if(childNodes, [node](const std::unique_ptr<Node>& n) {
            return n.get() == node;
        });
    }

    void Node::Tick() const
    {
        for (const auto& child : this->childNodes)
        {
            child->Tick();
        }

        for (const auto& script : this->scripts)
        {
            script->Tick();
        }
    }
}
