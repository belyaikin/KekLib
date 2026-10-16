//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include <memory>
#include <vector>

#include "Component.hpp"
#include "Transform.hpp"

namespace Kek::World
{
    class Node final
    {
        Node *parentNode = nullptr;
        std::vector<std::unique_ptr<Node>> childNodes;

        std::vector<std::unique_ptr<Component>> components;

        Transform* transform;
    public:
        Node()
        {
            this->transform = AttachComponent<Transform>(Math::Vector3{0, 0, 0});
        }

        ~Node() = default;

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

        template <typename T, typename... Args>
        T* AttachComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            auto component = std::make_unique<T>(std::forward<Args>(args)...);

            T* raw = component.get();
            raw->SetNode(this);

            this->components.push_back(std::move(component));

            return raw;
        }

        [[nodiscard]] Transform* GetTransform() const { return transform; }

        void Start();
        void Tick();
    };
}
