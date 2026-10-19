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
    /// @brief The building block of a game world.
    class Node final
    {
        Node *parentNode = nullptr;
        std::vector<std::unique_ptr<Node>> children;

        bool markedForRemoval = false;
        std::vector<std::unique_ptr<Node>> pendingChildren;

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
        const std::vector<std::unique_ptr<Node>>& GetChildren() const
        {
            return children;
        }

        /// @brief Finds the child nodes of the specified type.
        /// @tparam T The type of the child nodes to find.
        /// @return A list of raw pointers to nodes.
        template <typename T>
        std::vector<T*> GetChildrenOfType() const
        {
            std::vector<T*> found = {};

            for (const auto& node : this->children)
            {
                if (auto child = dynamic_cast<T*>(node.get()))
                    found.push_back(child);
            }

            return found;
        }

        /// @brief Immediately adds a child node without flushing.
        ///
        /// Use this method when you're assembling a node hierarchy before runtime.
        void AddChild(std::unique_ptr<Node> node);

        /// @brief Adds node to the addition queue.
        ///
        /// Use this method during runtime to spawn nodes.
        void QueueAddChild(std::unique_ptr<Node> node);

        /// @brief Mark the current node for deletion during the next flush.
        void QueueFree(Node *node);

        [[nodiscard]] bool IsQueuedForRemoval() const { return markedForRemoval; }

        /// @brief Attaches a component to the current node.
        /// @tparam T The type of component to attach (must derive from Component).
        /// @param args The arguments forwarded to the component's constructor.
        /// @return A raw pointer to the newly attached component instance.
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

        /// @brief Recursively clears the queues.
        void Flush();
    };
}
