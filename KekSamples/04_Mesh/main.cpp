//
// Created by readmao on 24.10.2026.
//
#include <iostream>
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/GpuMesh.hpp"
#include "KekGraphics/Model.hpp"
#include "KekGraphics/OpenGL/Context.hpp"

using namespace Kek::Graphics;

struct MeshCheck final : Kek::World::Component
{
    void Start() override
    {
        const GpuMesh mesh{Model::Triangle()};

        std::cout << "mesh uploaded, element count = " << mesh.GetCount() << " (expected 3)\n";
        std::cout << "glGetError: " << OpenGL::Context::GetError() << " (expected 0)\n";
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();
    root->AttachComponent<MeshCheck>();

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 04 Mesh"}, std::move(root));
    app.Run();
    return 0;
}