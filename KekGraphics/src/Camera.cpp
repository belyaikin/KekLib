//
// Created by readmaooo on 20.10.2026.
//

#include "KekGraphics/Components/Camera.hpp"

#include "KekWorld/Node.hpp"

namespace Kek::Graphics::Components
{
    Math::Matrix4 Camera::GetViewMatrix() const
    {
        const World::Node* node = GetNode();
        if (node == nullptr) return Math::Matrix4::Identity();

        const World::Transform* transform = node->GetTransform();

        // The view matrix is the inverse of the camera's own transform (rotate * translate).
        // The inverse of a rotation is its transpose, the inverse of a translation is the opposite translation.
        const Math::Matrix4 rotation = Math::Matrix4::Rotation(transform->GetRotation());

        return rotation.Transposed() * Math::Matrix4::Translation(-transform->GetPosition());
    }

    Math::Matrix4 Camera::GetProjectionMatrix(const float aspectRatio) const
    {
        return Math::Matrix4::Perspective(verticalFov, aspectRatio, nearPlane, farPlane);
    }
}