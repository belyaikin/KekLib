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

        // Please don't use this function inside a components code
        // because it can be buggy
        void SetNode(Node *node) { this->node = node; }

        virtual void Start();
        virtual void Tick();
    };
}
