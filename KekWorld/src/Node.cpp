//
// Created by Dmitriy on 21.09.2026.
//

#include "KekWorld/Nodes/Node.hpp"

namespace Kek::Nodes
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
}
