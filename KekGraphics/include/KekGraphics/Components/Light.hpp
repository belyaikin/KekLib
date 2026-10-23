//
// Created by vladk on 23.10.2026.
//

#pragma once

#include "KekMath/Vector3.hpp"
#include "KekWorld/Component.hpp"
#include "KekWorld/Node.hpp"

namespace Kek::Graphics::Components
{
    class Light final : public World::Component
    {
        Math::Vector3 color;
        float intensity;

    public:
        explicit Light(const Math::Vector3 color = {1, 1, 1}, const float intensity = 1.0f)
            : color(color), intensity(intensity) {}

        [[nodiscard]] Math::Vector3 GetColor() const { return color * intensity; }
        void SetColor(const Math::Vector3& c) { color = c; }
        void SetIntensity(const float i) { intensity = i; }

        // Position comes from the node's Transform
        [[nodiscard]] Math::Vector3 GetPosition() const
        {
            return GetNode()->GetTransform()->GetPosition();
        }
    };
}
