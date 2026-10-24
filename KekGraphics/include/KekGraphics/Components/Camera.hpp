//
// Created by Dmitriy on 20.10.2026.
//

#pragma once

#include "KekMath/Matrix4.hpp"
#include "KekWorld/Component.hpp"

namespace Kek::Graphics::Components
{
    /// @brief A perspective camera. It looks along the -Z axis of its node's Transform
    /// (so with zero rotation it looks "into" the screen). The Renderer uses the first camera it finds.
    class Camera final : public World::Component
    {
        float verticalFov;
        float nearPlane;
        float farPlane;

    public:
        /// @param verticalFov Vertical field of view in radians (default: 60 degrees).
        explicit Camera(const float verticalFov = 1.0471976f,
                        const float nearPlane = 0.1f,
                        const float farPlane = 100.0f)
            : verticalFov(verticalFov), nearPlane(nearPlane), farPlane(farPlane) {}

        [[nodiscard]] float GetVerticalFov() const { return verticalFov; }
        void SetVerticalFov(const float radians) { verticalFov = radians; }

        /// @brief World -> camera space. Uses the node's position and rotation (scale and parents are ignored).
        [[nodiscard]] Math::Matrix4 GetViewMatrix() const;

        [[nodiscard]] Math::Matrix4 GetProjectionMatrix(float aspectRatio) const;
    };
}