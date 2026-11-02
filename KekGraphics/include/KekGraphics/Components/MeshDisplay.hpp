//
// Created by Dmitriy on 16.10.2026.
//

#pragma once

#include <memory>
#include <utility>

#include "KekGraphics/Material.hpp"
#include "KekGraphics/MeshBuffers.hpp"
#include "KekGraphics/Mesh.hpp"
#include "KekWorld/Component.hpp"

namespace Kek::Graphics::Components
{
    /// @brief Registers this node's mesh and world transform with the render registry.
    class MeshDisplay final : public World::Component
    {
        Mesh model;
        MaterialSettings materialSettings;
        std::unique_ptr<MeshBuffers> buffers;
        std::unique_ptr<Material> material;


    public:
        explicit MeshDisplay(Mesh model, MaterialSettings settings = {})
            : model(std::move(model)), materialSettings(std::move(settings)) {}

        /// @brief Uploads the mesh to GPU buffers and registers it for rendering.
        void Start() override;
        void Destroy() override;

        /// @brief The GPU buffers, or nullptr before Start().
        [[nodiscard]] const MeshBuffers* GetBuffers() const { return buffers.get(); }
    };
}
