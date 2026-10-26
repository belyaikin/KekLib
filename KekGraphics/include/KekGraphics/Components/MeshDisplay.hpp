//
// Created by Dmitriy on 16.10.2026.
//

#pragma once

#include <memory>
#include <utility>

#include "KekGraphics/MeshBuffers.hpp"
#include "KekGraphics/Mesh.hpp"
#include "KekWorld/Component.hpp"

namespace Kek::Graphics::Components
{
    /// @brief Registers this node's mesh and world transform with the render registry.
    class MeshDisplay final : public World::Component
    {
        Mesh model;
        std::unique_ptr<MeshBuffers> buffers;

    public:
        explicit MeshDisplay(Mesh model) : model(std::move(model)) {}

        /// @brief Uploads the mesh to GPU buffers and registers it for rendering.
        void Start() override;
        void Destroy() override;

        /// @brief The GPU buffers, or nullptr before Start().
        [[nodiscard]] const MeshBuffers* GetBuffers() const { return buffers.get(); }
    };
}
