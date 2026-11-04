//
// Created by Dmitriy on 27.10.2026.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/Light.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekInput/Input.hpp"
#include "KekInput/Key.hpp"
#include "KekWorld/Node.hpp"
#include "KekWorld/Transform.hpp"

using namespace Kek::Graphics;
using Kek::World::Node;
using Kek::World::Transform;
using Kek::Input::Key;
using Kek::Math::Vector3;

namespace
{
    constexpr float FieldHalfHeight = 2.0f;
    constexpr float WallY = 2.5f;
    constexpr float OutX = 3.5f;

    bool Overlaps(const Node* nodeA, const Node* nodeB)
    {
        if (nodeA == nullptr || nodeB == nullptr)
            return false;

        const auto* a = nodeA->GetComponent<Transform>();
        const auto* b = nodeB->GetComponent<Transform>();
        if (a == nullptr || b == nullptr)
            return false;

        const Vector3 aPos = a->GetPosition(), bPos = b->GetPosition();
        const Vector3 aScale = a->GetScale(), bScale = b->GetScale();

        return std::abs(bPos.X() - aPos.X()) <= (aScale.X() + bScale.X()) * 0.5f &&
               std::abs(bPos.Y() - aPos.Y()) <= (aScale.Y() + bScale.Y()) * 0.5f;
    }
}

struct Ball final : Kek::World::Component
{
    static constexpr float StartSpeed = 0.08f;

    Vector3 direction{1, 0.6f, 0};
    float speed = StartSpeed;
    int playerScore = 0;
    int enemyScore = 0;

    void Reset(const float towardsX)
    {
        GetNode()->GetComponent<Transform>()->SetPosition(Vector3{0, 0, 0});
        direction = Vector3{towardsX, 0.6f, 0};
        speed = StartSpeed;
    }

    void PrintScore() const
    {
        std::cout << "Player " << playerScore << " : " << enemyScore << " Enemy\n";
    }

    void Tick() override
    {
        auto* transform = GetNode()->GetComponent<Transform>();
        Vector3 position = transform->GetPosition();

        if ((position.Y() <= -WallY && direction.Y() < 0.0f) ||
            (position.Y() >= WallY && direction.Y() > 0.0f))
        {
            direction = Vector3{direction.X(), -direction.Y(), 0};
        }

        position = position + direction.Normalized() * speed;
        transform->SetPosition(position);

        speed += 0.00002f;

        if (position.X() > OutX)
        {
            ++playerScore;
            PrintScore();
            Reset(-1.0f);
        }
        else if (position.X() < -OutX)
        {
            ++enemyScore;
            PrintScore();
            Reset(1.0f);
        }
    }
};

static void BounceOffPaddle(const Node* paddle, Node* ballNode, const float outward)
{
    auto* ball = ballNode->GetComponent<Ball>();
    if (ball == nullptr || !Overlaps(paddle, ballNode))
        return;

    if (ball->direction.X() * outward >= 0.0f)
        return;

    const auto* paddleTransform = paddle->GetComponent<Transform>();
    const float paddleY = paddleTransform->GetPosition().Y();
    const float paddleHalfHeight = paddleTransform->GetScale().Y() * 0.5f;
    const float ballY = ballNode->GetComponent<Transform>()->GetPosition().Y();

    const float offset = std::clamp((ballY - paddleY) / paddleHalfHeight, -1.0f, 1.0f);
    ball->direction = Vector3{outward, offset * 0.8f, 0};
}

struct PlayerPaddleController final : Kek::World::Component
{
    float speed = 0.06f;
    Node* ball = nullptr;

    void Tick() override
    {
        auto* transform = GetNode()->GetComponent<Transform>();
        const Vector3 position = transform->GetPosition();

        float direction = 0.0f;
        if (Kek::Input::GetKey(Key::W)) direction += 1.0f;
        if (Kek::Input::GetKey(Key::S)) direction -= 1.0f;

        transform->SetPosition(Vector3{
            position.X(),
            std::clamp(position.Y() + direction * speed, -FieldHalfHeight, FieldHalfHeight),
            position.Z()
        });

        if (ball != nullptr) BounceOffPaddle(GetNode(), ball, 1.0f);
    }
};

struct PlayerPaddleController2 final : Kek::World::Component
{
    float speed = 0.06f;
    Node* ball = nullptr;

    void Tick() override
    {
        auto* transform = GetNode()->GetComponent<Transform>();
        const Vector3 position = transform->GetPosition();

        float direction = 0.0f;
        if (Kek::Input::GetKey(Key::Up)) direction += 1.0f;
        if (Kek::Input::GetKey(Key::Down)) direction -= 1.0f;

        transform->SetPosition(Vector3{
            position.X(),
            std::clamp(position.Y() + direction * speed, -FieldHalfHeight, FieldHalfHeight),
            position.Z()
        });

        BounceOffPaddle(GetNode(), ball, -1.0f);
    }
};


struct LightFollow final : Kek::World::Component
{
    Node* target = nullptr;
    Vector3 offset{0, 0, 2};

    void Tick() override
    {
        if (target == nullptr) return;

        const Vector3 targetPosition = target->GetComponent<Transform>()->GetPosition();
        GetNode()->GetComponent<Transform>()->SetPosition(targetPosition + offset);
    }
};

static std::unique_ptr<Node> MakePaddle(const float x, const Vector3& color)
{
    auto paddle = std::make_unique<Node>();
    paddle->AttachComponent<Transform>(Vector3{x, 0, 0})->SetScale(Vector3{0.3f, 1, 0.3f});
    paddle->AttachComponent<Components::MeshDisplay>(Mesh::Cube(color));
    return paddle;
}

int main()
{
    auto root = std::make_unique<Node>();

    auto ball = std::make_unique<Node>();
    Node* ballNode = ball.get();
    ball->AttachComponent<Transform>(Vector3{0, 0, 0})->SetScale(Vector3{0.3f, 0.3f, 0.3f});
    ball->AttachComponent<Components::MeshDisplay>(Mesh::Cube(Vector3{1.0f, 0.85f, 0.2f}));
    ball->AttachComponent<Ball>();
    root->AddChild(std::move(ball));

    auto playerPaddle = MakePaddle(-2.5f, Vector3{0.3f, 0.8f, 1.0f});
    playerPaddle->AttachComponent<PlayerPaddleController>()->ball = ballNode;
    root->AddChild(std::move(playerPaddle));

    auto enemyPaddle = MakePaddle(2.5f, Vector3{1.0f, 0.35f, 0.35f});
    enemyPaddle->AttachComponent<PlayerPaddleController2>()->ball = ballNode;
    root->AddChild(std::move(enemyPaddle));

    auto light = std::make_unique<Node>();
    light->AttachComponent<Transform>(Vector3{0, 0, 2});
    light->AttachComponent<Components::Light>(Vector3{1, 1, 1}, 1.0f);
    light->AttachComponent<LightFollow>()->target = ballNode;
    root->AddChild(std::move(light));

    auto camera = std::make_unique<Node>();
    camera->AttachComponent<Transform>(Vector3{0, 0, 5});
    camera->AttachComponent<Components::Camera>();
    root->AddChild(std::move(camera));

    Kek::Application app(
        Kek::Platform::WindowSettings{
            .width = 640,
            .height = 640,
            .title = "KekLib - 14 Pong"
        },
        std::move(root)
    );

    app.SetTickRate(60);
    app.Run();
    return 0;
}