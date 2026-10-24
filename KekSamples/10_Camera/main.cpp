//
// Created by readmao on 24.10.2026.
//
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"

using namespace Kek::Graphics;
using Kek::Math::Vector3;

// Rotates its node around the Y axis: a flat triangle turns edge-on and back.
struct SpinY final : Kek::World::Component
{
    void Tick() override
    {
        GetNode()->GetTransform()->Rotate(Vector3{0, 0.03f, 0});
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();

    // The camera stands 3 units away from the origin and looks at it (-Z is "forward").
    auto camera = std::make_unique<Kek::World::Node>();
    camera->GetTransform()->SetPosition(Vector3{0, 0, 3});
    camera->AttachComponent<Components::Camera>();
    root->AddChild(std::move(camera));

    // A static 1x1 quad: with a correct aspect ratio it must look square, not stretched to 4:3.
    auto quad = std::make_unique<Kek::World::Node>();
    quad->GetTransform()->SetPosition(Vector3{-0.8f, 0, 0});
    quad->AttachComponent<Components::MeshDisplay>(Model::Quad());
    root->AddChild(std::move(quad));

    // A spinning triangle further to the right.
    auto triangle = std::make_unique<Kek::World::Node>();
    triangle->GetTransform()->SetPosition(Vector3{0.8f, 0, 0});
    triangle->AttachComponent<Components::MeshDisplay>(Model::Triangle());
    triangle->AttachComponent<SpinY>();
    root->AddChild(std::move(triangle));

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 10 Camera"}, std::move(root));
    app.SetTickRate(60);
    app.Run();
    return 0;
}