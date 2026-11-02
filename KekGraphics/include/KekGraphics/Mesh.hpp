//
// Created by Dmitriy on 29.09.2026.
//

#pragma once
#include <array>
#include <cstddef>
#include <utility>
#include <vector>

#include "Vertex.hpp"

namespace Kek::Graphics
{
    /// @brief CPU-side geometry. If there are no indices, vertices are drawn in order.
    class Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

    public:
        explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices = {})
            : vertices(std::move(vertices)), indices(std::move(indices)) {}

        [[nodiscard]] const std::vector<Vertex>& GetVertices() const { return vertices; }
        [[nodiscard]] const std::vector<unsigned int>& GetIndices() const { return indices; }

        /// @brief A colored triangle in the XY plane, 1 unit wide.
        [[nodiscard]] static Mesh Triangle();
        /// @brief A colored 1x1 quad in the XY plane, built from 4 vertices and 6 indices.
        [[nodiscard]] static Mesh Quad();
        /// @brief A cube with side 1, one color per face (24 vertices with flat normals, 36 indices).
        [[nodiscard]] static Mesh Cube();
        /// @brief The same cube painted in a single color.
        [[nodiscard]] static Mesh Cube(const Math::Vector3& color);
    };
}