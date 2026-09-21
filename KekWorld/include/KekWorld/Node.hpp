//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include <memory>
#include <vector>

namespace Kek::World
{
    class Scene;

    struct Position {
        float x, y, z;
    };

    struct Rotation {
        float x, y, z;
    };

    class Node {
        const Scene *scene = nullptr;

        Node *parentNode = nullptr;
        std::vector<std::unique_ptr<Node>> childNodes;

        Position position = {.x = 0, .y = 0, .z = 0};
        Rotation rotation = {.x = 0, .y = 0, .z = 0};

    public:
        Node() = default;

        virtual ~Node() = default;

        [[nodiscard]]
        const Scene *GetScene() const
        {
            return scene;
        }

        void SetScene(const Scene *scene)
        {
            this->scene = scene;
        }

        [[nodiscard]]
        Node *GetParentNode() const
        {
            return parentNode;
        }

        [[nodiscard]]
        const std::vector<std::unique_ptr<Node>>& GetChildNodes() const
        {
            return childNodes;
        }

        template <typename T>
        std::vector<T*> GetChildNodesOfType() const
        {
            std::vector<T*> found = {};

            for (const auto& node : this->childNodes)
            {
                if (auto child = dynamic_cast<T*>(node.get()))
                    found.push_back(child);
            }

            return found;
        }

        void AddChildNode(std::unique_ptr<Node> node);
        void RemoveChildNode(Node *node);

        [[nodiscard]]
        Position GetPosition() const
        {
            return this->position;
        }

        void SetPosition(const Position& position)
        {
            this->position = position;
        }

        [[nodiscard]]
        Rotation GetRotation() const
        {
            return this->rotation;
        }

        void SetRotation(const Rotation& rotation)
        {
            this->rotation = rotation;
        }

        virtual void Start() {}
        virtual void Update() {}
    };
}
