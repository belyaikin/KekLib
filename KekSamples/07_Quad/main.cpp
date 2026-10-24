//
// Created by readmao on 24.10.2026.
//
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"

using namespace Kek::Graphics;

int main()
{
    auto root = std::make_unique<Kek::World::Node>();
    root->AttachComponent<Components::MeshDisplay>(Model::Quad());

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 07 Quad (indexed)"}, std::move(root));
    app.Run();
    return 0;
}
