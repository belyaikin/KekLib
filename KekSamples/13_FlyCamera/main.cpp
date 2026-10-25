//
// Created by readmao on 25.10.2026.
//
#include <algorithm>
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekInput/Input.hpp"
#include "KekMath/Matrix4.hpp"

using namespace Kek::Graphics;
using Kek::Input::Key;
using Kek::Math::Matrix4;
using Kek::Math::Vector3;

// W/S - forward/back, A/D - strafe, Space/LeftShift - up/down, arrow keys - look around.
struct FlyCamera final : Kek::World::Component
{
    static constexpr float MoveSpeed = 0.06f;
    static constexpr float TurnSpeed = 0.025f;

    void Tick() override
    {
        auto* transform = GetNode()->GetTransform();

        Vector3 rotation = transform->GetRotation();
        if (Kek::Input::GetKey(Key::Left)) rotation += Vector3{0, TurnSpeed, 0};
        if (Kek::Input::GetKey(Key::Right)) rotation += Vector3{0, -TurnSpeed, 0};
        if (Kek::Input::GetKey(Key::Up)) rotation += Vector3{TurnSpeed, 0, 0};
        if (Kek::Input::GetKey(Key::Down)) rotation += Vector3{-TurnSpeed, 0, 0};

        rotation = Vector3{std::clamp(rotation.X(), -1.5f, 1.5f), rotation.Y(), rotation.Z()};
        transform->SetRotation(rotation);

        // The camera looks along its local -Z; rotate the local axes into world space.
        const Matrix4 orientation = Matrix4::Rotation(rotation);
        const Vector3 forward = orientation.TransformPoint(Vector3{0, 0, -1});
        const Vector3 right = orientation.TransformPoint(Vector3{1, 0, 0});

        Vector3 move{0, 0, 0};
        if (Kek::Input::GetKey(Key::W)) move += forward;
        if (Kek::Input::GetKey(Key::S)) move += -forward;
        if (Kek::Input::GetKey(Key::D)) move += right;
        if (Kek::Input::GetKey(Key::A)) move += -right;
        if (Kek::Input::GetKey(Key::Space)) move += Vector3::up;
        if (Kek::Input::GetKey(Key::LeftShift)) move += Vector3::down;

        transform->Move(move * MoveSpeed);
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();

    auto camera = std::make_unique<Kek::World::Node>();
    camera->GetTransform()->SetPosition(Vector3{0, 1.5f, 6});
    camera->AttachComponent<Components::Camera>();
    camera->AttachComponent<FlyCamera>();
    root->AddChild(std::move(camera));

    // A 7x7 field of cubes of different heights to fly around.
    for (int x = -3; x <= 3; ++x)
    {
        for (int z = -3; z <= 3; ++z)
        {
            auto cube = std::make_unique<Kek::World::Node>();
            const float height = 0.5f + static_cast<float>((x * 3 + z * 5 + 20) % 4) * 0.5f;

            cube->GetTransform()->SetPosition(Vector3{x * 2.0f, height * 0.5f - 0.5f, z * 2.0f - 6.0f});
            cube->GetTransform()->SetScale(Vector3{1.0f, height, 1.0f});
            cube->AttachComponent<Components::MeshDisplay>(Model::Cube());

            root->AddChild(std::move(cube));
        }
    }

    Kek::Application app(Kek::Platform::WindowSettings{1024, 640, "KekLib - 13 Fly camera"}, std::move(root));
    app.SetTickRate(60);
    app.Run();
    return 0;
}