//
// Created by Dmitriy on 29.09.2026.
//

#pragma once
#include <vector>

#include "Vertex.hpp"

namespace Kek::Graphics
{
    class Model
    {
        std::vector<Vertex> vertices;
        
    public:
        Model() = default;
        explicit Model(std::vector<Vertex> vertices) : vertices(std::move(vertices)) {}

        [[nodiscard]] const std::vector<Vertex>& GetVertices() const { return vertices; }
    };
}
