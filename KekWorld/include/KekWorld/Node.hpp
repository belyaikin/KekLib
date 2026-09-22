//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include <memory>
#include <vector>

#include <KekMath/Vector3.hpp>

namespace Kek::World
{
    class Scene;

    class Node
    {
        const Scene *scene = nullptr;

        Node *parentNode = nullptr;
        std::vector<std::unique_ptr<Node>> childNodes;

        Math::Vector3 position = Math::Vector3(0, 0, 0);
        Math::Vector3 rotation = Math::Vector3(0, 0, 0);

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
        Math::Vector3 GetPosition() const
        {
            return this->position;
        }

        void SetPosition(const Math::Vector3& position)
        {
            this->position = position;
        }

        [[nodiscard]]
        Math::Vector3 GetRotation() const
        {
            return this->rotation;
        }

        void SetRotation(const Math::Vector3& rotation)
        {
            this->rotation = rotation;
        }

        virtual void Start() {}
        virtual void Update() {}
    };
}
