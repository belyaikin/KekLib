//
// Created by readmao on 24.10.2026.
//
#include "KekGraphics/Model.hpp"

namespace Kek::Graphics
{
    using Math::Vector3;

    Model Model::Triangle()
    {
        return Model{{
            Vertex{Vector3{-0.5f, -0.5f, 0.0f}, Vector3{1, 0, 0}},
            Vertex{Vector3{ 0.5f, -0.5f, 0.0f}, Vector3{0, 1, 0}},
            Vertex{Vector3{ 0.0f,  0.5f, 0.0f}, Vector3{0, 0, 1}},
        }};
    }

    Model Model::Quad()
    {
        return Model{
            {
                Vertex{Vector3{-0.5f, -0.5f, 0.0f}, Vector3{1, 0, 0}},
                Vertex{Vector3{ 0.5f, -0.5f, 0.0f}, Vector3{0, 1, 0}},
                Vertex{Vector3{ 0.5f,  0.5f, 0.0f}, Vector3{0, 0, 1}},
                Vertex{Vector3{-0.5f,  0.5f, 0.0f}, Vector3{1, 1, 0}},
            },
            {0, 1, 2, 2, 3, 0}
        };
    }
}
