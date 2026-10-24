//
// Created by readmao on 24.10.2026.
//
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"

using namespace Kek::Graphics;
using Kek::Math::Vector3;

// Rotates its node around Z a little every tick.
struct Spinner final : Kek::World::Component
{
    void Tick() override
    {
        GetNode()->GetTransform()->Rotate(Vector3{0, 0, 0.03f});
    }
};

int main()
{
    // A big triangle that spins...
    auto root = std::make_unique<Kek::World::Node>();
    root->AttachComponent<Components::MeshDisplay>(Model::Triangle());
    root->AttachComponent<Spinner>();

    // ...and a small quad that is its child: it must orbit together with the parent.
    auto child = std::make_unique<Kek::World::Node>();
    child->GetTransform()->SetPosition(Vector3{0.7f, 0.0f, 0.0f});
    child->GetTransform()->SetScale(Vector3{0.25f, 0.25f, 1.0f});
    child->AttachComponent<Components::MeshDisplay>(Model::Quad());
    root->AddChild(std::move(child));

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 09 Transform"}, std::move(root));
    app.SetTickRate(60); // smoother motion than the default 20 ticks per second
    app.Run();
    return 0;
}
