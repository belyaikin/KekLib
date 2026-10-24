//
// Created by Dmitriy on 29.09.2026.
//

#pragma once

namespace Kek::World
{
    class Node;

    class Component
    {
        Node* node = nullptr;

    public:
        virtual ~Component() = default;

        // Please don't use this function inside of a components code
        // because it can be buggy
        void SetNode(Node *node) { this->node = node; }

        /// @brief The node this component is attached to.
        [[nodiscard]] Node* GetNode() const { return node; }

        virtual void Start() {}
        virtual void Tick() {}
    };
}
