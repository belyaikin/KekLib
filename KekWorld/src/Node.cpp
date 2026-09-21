//
// Created by Dmitriy on 21.09.2026.
//

#include "KekWorld/Node.hpp"

namespace Kek::World
{
    void Node::AddChildNode(std::unique_ptr<Node> node)
    {
        node->scene = scene;
        node->parentNode = this;
        childNodes.push_back(std::move(node));
    }
}