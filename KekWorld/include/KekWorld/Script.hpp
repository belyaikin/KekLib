//
// Created by Dmitriy on 29.09.2026.
//

#pragma once

namespace Kek::World
{
    class Node;

    class Script
    {
        Node* node = nullptr;

    public:
        virtual ~Script() = default;

        void SetNode(Node *node) { this->node = node; }

        virtual void Tick();
    };
}
