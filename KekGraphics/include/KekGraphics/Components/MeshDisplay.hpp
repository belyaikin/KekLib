//
// Created by Dmitriy on 16.10.2026.
//

#pragma once

#include <memory>
#include <utility>

#include "KekGraphics/GpuMesh.hpp"
#include "KekGraphics/Model.hpp"
#include "KekWorld/Component.hpp"

namespace Kek::Graphics::Components
{
    /// @brief Makes a node visible: the Renderer draws the mesh of every MeshDisplay in the tree.
    class MeshDisplay final : public World::Component
    {
        Model model;
        std::unique_ptr<GpuMesh> mesh;

    public:
        explicit MeshDisplay(Model model) : model(std::move(model)) {}

        /// @brief Uploads the model to the GPU (the OpenGL context is ready when Start() is called).
        void Start() override;

        /// @brief The GPU mesh, or nullptr before Start().
        [[nodiscard]] const GpuMesh* GetMesh() const { return mesh.get(); }
    };
}