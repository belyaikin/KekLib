//
// Created by readmao on 24.10.2026.
//
#include "KekGraphics/Mesh.hpp"

namespace Kek::Graphics
{
    using Math::Vector3;

    Mesh Mesh::Triangle()
    {
        return Mesh{{
            Vertex{Vector3{-0.5f, -0.5f, 0.0f}, Vector3{1, 0, 0}},
            Vertex{Vector3{ 0.5f, -0.5f, 0.0f}, Vector3{0, 1, 0}},
            Vertex{Vector3{ 0.0f,  0.5f, 0.0f}, Vector3{0, 0, 1}},
        }};
    }

    Mesh Mesh::Quad()
    {
        return Mesh{
            {
                Vertex{Vector3{-0.5f, -0.5f, 0.0f}, Vector3{1, 0, 0}},
                Vertex{Vector3{ 0.5f, -0.5f, 0.0f}, Vector3{0, 1, 0}},
                Vertex{Vector3{ 0.5f,  0.5f, 0.0f}, Vector3{0, 0, 1}},
                Vertex{Vector3{-0.5f,  0.5f, 0.0f}, Vector3{1, 1, 0}},
            },
            {0, 1, 2, 2, 3, 0}
        };
    }

    Mesh Mesh::Cube()
    {
        return Mesh{
            {
                Vertex{Vector3{-0.5f, -0.5f,  0.5f}, Vector3{1.0f, 0.0f, 0.0f}}, // 0
                Vertex{Vector3{ 0.5f, -0.5f,  0.5f}, Vector3{0.0f, 1.0f, 0.0f}}, // 1
                Vertex{Vector3{ 0.5f,  0.5f,  0.5f}, Vector3{0.0f, 0.0f, 1.0f}}, // 2
                Vertex{Vector3{-0.5f,  0.5f,  0.5f}, Vector3{1.0f, 1.0f, 0.0f}}, // 3
                Vertex{Vector3{-0.5f, -0.5f, -0.5f}, Vector3{1.0f, 0.0f, 1.0f}}, // 4
                Vertex{Vector3{ 0.5f, -0.5f, -0.5f}, Vector3{0.0f, 1.0f, 1.0f}}, // 5
                Vertex{Vector3{ 0.5f,  0.5f, -0.5f}, Vector3{1.0f, 1.0f, 1.0f}}, // 6
                Vertex{Vector3{-0.5f,  0.5f, -0.5f}, Vector3{0.3f, 0.3f, 0.3f}}, // 7
            },
            {
                0, 1, 2,  2, 3, 0, // front  (+Z)
                5, 4, 7,  7, 6, 5, // back   (-Z)
                1, 5, 6,  6, 2, 1, // right  (+X)
                4, 0, 3,  3, 7, 4, // left   (-X)
                3, 2, 6,  6, 7, 3, // top    (+Y)
                4, 5, 1,  1, 0, 4, // bottom (-Y)
            }
        };
    }
}
