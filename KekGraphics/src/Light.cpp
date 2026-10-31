//
// Created by vladk on 31.10.2026.
//

#include "KekGraphics/Components/Light.hpp"
#include "KekGraphics/RenderRegistry.hpp"
#include "KekWorld/Node.hpp"
#include "KekWorld/Transform.hpp"

namespace Kek::Graphics::Components
{
    void Light::Start()
    {
        RenderRegistry::RegisterLight(this);
    }

    void Light::Destroy()
    {
        RenderRegistry::UnregisterLight(this);
    }

    Math::Vector3 Light::GetPosition() const
    {
        const World::Node* node = GetNode();

        if (node == nullptr) return Math::Vector3{0, 0, 0};

        const World::Transform* transform = node->GetComponent<World::Transform>();

        if (transform == nullptr) return Math::Vector3{0, 0, 0};

        return transform->GetWorldMatrix().TransformPoint(Math::Vector3{0, 0, 0});
    }
}