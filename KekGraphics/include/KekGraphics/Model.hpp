//
// Created by Dmitriy on 29.09.2026.
//

#pragma once
#include <utility>
#include <vector>

#include "Vertex.hpp"

namespace Kek::Graphics
{
    /// @brief CPU-side geometry. If there are no indices, vertices are drawn in order.
    class Model
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

    public:
        explicit Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices = {})
            : vertices(std::move(vertices)), indices(std::move(indices)) {}

        [[nodiscard]] const std::vector<Vertex>& GetVertices() const { return vertices; }
        [[nodiscard]] const std::vector<unsigned int>& GetIndices() const { return indices; }

        /// @brief A colored triangle in the XY plane, 1 unit wide.
        [[nodiscard]] static Model Triangle();
        /// @brief A colored 1x1 quad in the XY plane, built from 4 vertices and 6 indices.
        [[nodiscard]] static Model Quad();
        /// @brief A colored cube with side 1, centered at the origin (8 vertices, 36 indices).
        [[nodiscard]] static Model Cube();
    };
}