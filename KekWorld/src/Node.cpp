//
// Created by Dmitriy on 21.09.2026.
//

#include "../include/KekWorld/Node.hpp"

#include <memory>

namespace Kek::World
{
    Node::~Node()
    {
        Destroy();
    }

    void Node::AddChild(std::unique_ptr<Node> node)
    {
        node->parentNode = this;
        children.push_back(std::move(node));
    }

    void Node::QueueAddChild(std::unique_ptr<Node> node)
    {
        pendingChildren.push_back(std::move(node));
    }

    void Node::QueueFree(Node *node)
    {
        node->markedForRemoval = true;
    }

    void Node::Start()
    {
        for (const auto& component : components) component->Start();
        for (const auto& child : children) child->Start();
    }

    void Node::Tick()
    {
        if (markedForRemoval) return;
        for (const auto& component : components) component->Tick();
        for (const auto& child : children) child->Tick();
    }

    void Node::Destroy()
    {
        if (destroyed) return;
        destroyed = true;

        for (const auto& component : components)
            component->Destroy();

        for (const auto& child : children)
            child->Destroy();

        for (const auto& child : pendingChildren)
            child->Destroy();
    }

    void Node::Flush()
    {
        std::erase_if(children, [](const std::unique_ptr<Node>& childNode) {
            if (childNode->markedForRemoval)
                childNode->Destroy();
            return childNode->markedForRemoval;
        });

        std::vector<std::unique_ptr<Node>> batch;
        batch.swap(pendingChildren);
        for (auto& node : batch)
        {
            Node* raw = node.get();
            AddChild(std::move(node));
            raw->Start();
        }

        for (const auto& child : children)
            child->Flush();
    }
}
