//
// Created by vladk on 31.10.2026.
//

#pragma once
#include "KekMath/Vector3.hpp"
#include "KekWorld/Component.hpp"
namespace Kek::Graphics::Components
{
    class Light final : public World::Component
    {
        Math::Vector3 color;
        float intensity;

    public:
        explicit Light(const Math::Vector3& color = Math::Vector3{1, 1, 1}, const float intensity = 1.0f) : color(color), intensity(intensity) {}

        void Start() override;

        void Destroy() override;

        [[nodiscard]] Math::Vector3 GetColor() const { return color; }

        void SetColor(const Math::Vector3& value) { color = value; }

        [[nodiscard]] float GetIntensity() const { return intensity; }

        void SetIntensity(const float value) { intensity = value; }

        /// @brief World-space position of the light.
        [[nodiscard]] Math::Vector3 GetPosition() const;
    };
}