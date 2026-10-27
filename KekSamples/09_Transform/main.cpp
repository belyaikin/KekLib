//
// Created by readmao on 24.10.2026.
//
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekWorld/Transform.hpp"

using namespace Kek::Graphics;
using Kek::Math::Vector3;

// Rotates its node around Z a little every tick.
struct Spinner final : Kek::World::Component
{
    void Tick() override
    {
        GetNode()->GetComponent<Kek::World::Transform>()->Rotate(Vector3{0, 0, 0.03f});
    }
};

int main()
{
    // A big triangle that spins...
    auto root = std::make_unique<Kek::World::Node>();
    root->AttachComponent<Kek::World::Transform>(Vector3{0, 0, 0});
    root->AttachComponent<Components::MeshDisplay>(Mesh::Triangle());
    root->AttachComponent<Spinner>();

    // ...and a small quad that is its child: it must orbit together with the parent.
    auto child = std::make_unique<Kek::World::Node>();
    auto* childTransform = child->AttachComponent<Kek::World::Transform>(Vector3{0.7f, 0.0f, 0.0f});
    childTransform->SetScale(Vector3{0.25f, 0.25f, 1.0f});
    child->AttachComponent<Components::MeshDisplay>(Mesh::Quad());
    root->AddChild(std::move(child));

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 09 Transform"}, std::move(root));
    app.SetTickRate(60); // smoother motion than the default 20 ticks per second
    app.Run();
    return 0;
}
