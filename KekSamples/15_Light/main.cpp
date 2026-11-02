//
// Created by vladk on 01.11.2026.
//

#include <cmath>
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/Light.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekWorld/Transform.hpp"

using namespace Kek::Graphics;
using Kek::World::Node;
using Kek::World::Transform;
using Kek::Math::Vector3;

struct Rotator final : Kek::World::Component
{
    Vector3 speed;
    explicit Rotator(const Vector3& speed) : speed(speed) {}

    void Tick() override
    {
        GetNode()->GetComponent<Transform>()->Rotate(speed);
    }
};
// Moves its node on a circle around the Y axis.
struct Orbit final : Kek::World::Component
{
    float angle = 0.0f;
    void Tick() override
    {
        angle += 0.02f;
        GetNode()->GetComponent<Transform>()->SetPosition(Vector3{std::cos(angle) * 2.5f, 1.0f, std::sin(angle) * 2.5f});
    }
};
int main()
{
    auto root = std::make_unique<Node>();
    auto camera = std::make_unique<Node>();

    camera->AttachComponent<Transform>(Vector3{0, 2.0f, 6})->SetRotation(Vector3{-0.3f, 0, 0});
    camera->AttachComponent<Components::Camera>();
    root->AddChild(std::move(camera));

    auto cube = std::make_unique<Node>();
    cube->AttachComponent<Transform>(Vector3{0, 0, 0});
    cube->AttachComponent<Components::MeshDisplay>(Mesh::Cube(Vector3{0.9f, 0.9f, 0.9f}));
    cube->AttachComponent<Rotator>(Vector3{0.005f, 0.01f, 0});
    root->AddChild(std::move(cube));

    auto floor = std::make_unique<Node>();
    floor->AttachComponent<Transform>(Vector3{0, -1.0f, 0})->SetScale(Vector3{8, 0.1f, 8});
    floor->AttachComponent<Components::MeshDisplay>(Mesh::Cube(Vector3{0.6f, 0.6f, 0.6f}));
    root->AddChild(std::move(floor));

    // The light and a small unlit marker cube so we can see where it is.
    auto light = std::make_unique<Node>();
    light->AttachComponent<Transform>(Vector3{2.5f, 1.0f, 0})->SetScale(Vector3{0.15f, 0.15f, 0.15f});
    light->AttachComponent<Components::MeshDisplay>(Mesh::Cube(Vector3{1, 1, 0.6f}), MaterialSettings{.fragmentShader = "shaders/unlit.frag"});
    light->AttachComponent<Components::Light>(Vector3{1, 1, 1}, 1.0f);
    light->AttachComponent<Orbit>();
    root->AddChild(std::move(light));

    Kek::Application app(Kek::Platform::WindowSettings{1024, 640, "KekLib - 15 Light"}, std::move(root));
    app.SetTickRate(60);
    app.Run();
    return 0;
}