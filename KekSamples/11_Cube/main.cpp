//
// Created by readmao on 25.10.2026.
//
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekWorld/Transform.hpp"

using namespace Kek::Graphics;
using Kek::Math::Vector3;

struct Rotator final : Kek::World::Component
{
    Vector3 speed;

    explicit Rotator(const Vector3& speed) : speed(speed) {}

    void Tick() override
    {
        GetNode()->GetComponent<Kek::World::Transform>()->Rotate(speed);
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();

    auto camera = std::make_unique<Kek::World::Node>();
    auto* cameraTransform = camera->AttachComponent<Kek::World::Transform>(Vector3{0, 1.0f, 4});
    cameraTransform->SetRotation(Vector3{-0.25f, 0, 0}); // look slightly down
    camera->AttachComponent<Components::Camera>();
    root->AddChild(std::move(camera));

    auto cube = std::make_unique<Kek::World::Node>();
    cube->AttachComponent<Kek::World::Transform>(Vector3{0, 0, 0});
    cube->AttachComponent<Components::MeshDisplay>(Mesh::Cube());
    cube->AttachComponent<Rotator>(Vector3{0.01f, 0.02f, 0});

    // A smaller cube orbiting the big one (it is a child, so it inherits the parent's rotation).
    auto satellite = std::make_unique<Kek::World::Node>();
    auto* satelliteTransform = satellite->AttachComponent<Kek::World::Transform>(Vector3{1.4f, 0, 0});
    satelliteTransform->SetScale(Vector3{0.4f, 0.4f, 0.4f});
    satellite->AttachComponent<Components::MeshDisplay>(Mesh::Cube());
    satellite->AttachComponent<Rotator>(Vector3{0.0f, 0.05f, 0.03f});
    cube->AddChild(std::move(satellite));

    root->AddChild(std::move(cube));

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 11 Cube"}, std::move(root));
    app.SetTickRate(60);
    app.Run();
    return 0;
}
