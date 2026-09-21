//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include <utility>
#include <vector>

#include "Scene.h"

namespace Kek::World {
    class Node {
        const Scene *scene = nullptr;

        Node *parentNode = nullptr;
        std::vector<std::unique_ptr<Node>> childNodes;

    public:
        Node() = default;

        virtual ~Node() = default;

        [[nodiscard]] const Scene *GetScene() const { return scene; }
        void SetScene(const Scene *scene) { this->scene = scene; }

        [[nodiscard]] Node *GetParentNode() const { return parentNode; }

        [[nodiscard]] const std::vector<std::unique_ptr<Node>>& GetChildNodes() const { return childNodes; }
        void AddChildNode(std::unique_ptr<Node> node) {
            node->scene = scene;
            node->parentNode = this;
            childNodes.push_back(std::move(node));
        }

        virtual void Start();
        virtual void Update();
    };
}
