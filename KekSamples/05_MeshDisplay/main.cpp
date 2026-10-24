//
// Created by readmao on 24.10.2026.
//
#include <iostream>
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"

using namespace Kek::Graphics;

// Attached AFTER MeshDisplay, so its Start() runs after the mesh was uploaded.
struct UploadCheck final : Kek::World::Component
{
    void Start() override
    {
        const auto* display = GetNode()->GetComponent<Components::MeshDisplay>();

        std::cout << "MeshDisplay found:      " << (display != nullptr ? "yes" : "NO") << '\n';
        std::cout << "mesh uploaded in Start: " << (display != nullptr && display->GetMesh() != nullptr ? "yes" : "NO") << '\n';
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();
    root->AttachComponent<Components::MeshDisplay>(Model::Triangle());
    root->AttachComponent<UploadCheck>();

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 05 MeshDisplay"}, std::move(root));
    app.Run();
    return 0;
}
