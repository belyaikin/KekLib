//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include <vector>

#include "Script.hpp"
#include "Transform.hpp"

namespace Kek::World
{
    class Node
    {
        Node *parentNode = nullptr;
        std::vector<std::unique_ptr<Node>> childNodes;

        std::vector<std::unique_ptr<Script>> scripts;

        Transform* transform;
    public:
        Node()
        {
            this->transform = AttachScript<Transform>(Math::Vector3{0, 0, 0});
        }

        virtual ~Node() = default;

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
        T* AttachScript(Args&&... args)
        {
            static_assert(std::is_base_of_v<Script, T>, "T must derive from Script");

            auto script = std::make_unique<T>(std::forward<Args>(args)...);

            T* raw = script.get();
            raw->SetNode(this);

            this->scripts.push_back(std::move(script));

            return raw;
        }

    private:
        void TickScripts() const;
    };
}
