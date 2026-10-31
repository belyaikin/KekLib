//
// Created by readmao on 24.10.2026.
//
#include "KekGraphics/Mesh.hpp"


namespace Kek::Graphics
{
    using Math::Vector2;
    using Math::Vector3;

    namespace
    {
        /// One flat-shaded face: 4 own vertices (own normal and UVs) and 2 triangles.
        void AddFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const Vector3& normal, const std::array<Vector3, 4>& corners, const Vector3& color)
        {
            const auto first = static_cast<unsigned int>(vertices.size());
            const std::array<Vector2, 4> uvs{Vector2{0, 0}, Vector2{1, 0}, Vector2{1, 1}, Vector2{0, 1}};

            for (std::size_t i = 0; i < 4; ++i)
                vertices.emplace_back(corners[i], normal, color, uvs[i]);

            indices.insert(indices.end(), {first, first + 1, first + 2, first + 2, first + 3, first});
        }

        Mesh BuildCube(const std::array<Vector3, 6>& faceColors)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            vertices.reserve(24);
            indices.reserve(36);

            // Corners are counter-clockwise when looking at the face from the outside.
            AddFace(vertices, indices, Vector3{0, 0, 1},  // front (+Z)
                    {Vector3{-0.5f, -0.5f, 0.5f}, Vector3{0.5f, -0.5f, 0.5f}, Vector3{0.5f, 0.5f, 0.5f}, Vector3{-0.5f, 0.5f, 0.5f}},
                    faceColors[0]);
            AddFace(vertices, indices, Vector3{0, 0, -1}, // back (-Z)
                    {Vector3{0.5f, -0.5f, -0.5f}, Vector3{-0.5f, -0.5f, -0.5f}, Vector3{-0.5f, 0.5f, -0.5f}, Vector3{0.5f, 0.5f, -0.5f}},
                    faceColors[1]);
            AddFace(vertices, indices, Vector3{1, 0, 0},  // right (+X)
                    {Vector3{0.5f, -0.5f, 0.5f}, Vector3{0.5f, -0.5f, -0.5f}, Vector3{0.5f, 0.5f, -0.5f}, Vector3{0.5f, 0.5f, 0.5f}},
                    faceColors[2]);
            AddFace(vertices, indices, Vector3{-1, 0, 0}, // left (-X)
                    {Vector3{-0.5f, -0.5f, -0.5f}, Vector3{-0.5f, -0.5f, 0.5f}, Vector3{-0.5f, 0.5f, 0.5f}, Vector3{-0.5f, 0.5f, -0.5f}},
                    faceColors[3]);
            AddFace(vertices, indices, Vector3{0, 1, 0},  // top (+Y)
                    {Vector3{-0.5f, 0.5f, 0.5f}, Vector3{0.5f, 0.5f, 0.5f}, Vector3{0.5f, 0.5f, -0.5f}, Vector3{-0.5f, 0.5f, -0.5f}},
                    faceColors[4]);
            AddFace(vertices, indices, Vector3{0, -1, 0}, // bottom (-Y)
                    {Vector3{-0.5f, -0.5f, -0.5f}, Vector3{0.5f, -0.5f, -0.5f}, Vector3{0.5f, -0.5f, 0.5f}, Vector3{-0.5f, -0.5f, 0.5f}},
                    faceColors[5]);

            return Mesh{std::move(vertices), std::move(indices)};
        }
    }

    Mesh Mesh::Triangle()
    {
        const Vector3 normal{0, 0, 1};
        return Mesh{{
            Vertex{Vector3{-0.5f, -0.5f, 0.0f}, normal, Vector3{1, 0, 0}, Vector2{0, 0}},
            Vertex{Vector3{ 0.5f, -0.5f, 0.0f}, normal, Vector3{0, 1, 0}, Vector2{1, 0}},
            Vertex{Vector3{ 0.0f,  0.5f, 0.0f}, normal, Vector3{0, 0, 1}, Vector2{0.5f, 1}},
        }};
    }

    Mesh Mesh::Quad()
    {
        const Vector3 normal{0, 0, 1};
        return Mesh{
            {
                Vertex{Vector3{-0.5f, -0.5f, 0.0f}, normal, Vector3{1, 0, 0}, Vector2{0, 0}},
                Vertex{Vector3{ 0.5f, -0.5f, 0.0f}, normal, Vector3{0, 1, 0}, Vector2{1, 0}},
                Vertex{Vector3{ 0.5f,  0.5f, 0.0f}, normal, Vector3{0, 0, 1}, Vector2{1, 1}},
                Vertex{Vector3{-0.5f,  0.5f, 0.0f}, normal, Vector3{1, 1, 0}, Vector2{0, 1}},
            },
            {0, 1, 2, 2, 3, 0}
        };
    }

    Mesh Mesh::Cube()
    {
        return BuildCube({
            Vector3{1.0f, 0.3f, 0.3f}, Vector3{0.3f, 1.0f, 0.3f}, Vector3{0.3f, 0.3f, 1.0f},
            Vector3{1.0f, 1.0f, 0.3f}, Vector3{1.0f, 0.3f, 1.0f}, Vector3{0.3f, 1.0f, 1.0f},
        });
    }

    Mesh Mesh::Cube(const Vector3& color)
    {
        return BuildCube({color, color, color, color, color, color});
    }
}