//
// Created by vladk on 08.11.2026.
//

#pragma once

#include <algorithm>
#include <array>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekMath/Vector3.hpp"
#include "KekWorld/Node.hpp"
#include "KekWorld/Transform.hpp"

namespace Hud
{
    using Kek::Math::Vector3;
    using Kek::World::Node;
    using Kek::World::Transform;

    constexpr float Hidden = -1000.0f;
    constexpr float Depth = -3.0f;

    inline Transform* MakeCube(Node& parent, const Vector3& color, const bool unlit,
                               const Vector3& scale = Vector3{1, 1, 1})
    {
        using namespace Kek::Graphics;

        auto node = std::make_unique<Node>();
        auto* transform = node->AttachComponent<Transform>(Vector3{0, Hidden, 0});
        transform->SetScale(scale);

        if (unlit)
            node->AttachComponent<Components::MeshDisplay>(Mesh::Cube(color), MaterialSettings{.fragmentShader = "shaders/unlit.frag"});
        else
            node->AttachComponent<Components::MeshDisplay>(Mesh::Cube(color));

        parent.AddChild(std::move(node));
        return transform;
    }
}