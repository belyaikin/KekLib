//
// Created by Dmitriy on 27.10.2026.
//

#include <memory>
#include <algorithm>

#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekInput/Input.hpp"
#include "KekInput/Key.hpp"
#include "KekWorld/Node.hpp"
#include "KekWorld/Transform.hpp"

using namespace Kek::Graphics;
using Kek::World::Node;
using Kek::World::Transform;
using Kek::Input::Key;
using Kek::Math::Matrix4;
using Kek::Math::Vector3;

static bool overlaps(const Node *nodeA, const Node *nodeB)
{
    if (nodeA == nullptr || nodeB == nullptr)
        return false;

    const auto* nodeATransform = nodeA->GetComponent<Transform>();
    const auto* nodeBTransform = nodeB->GetComponent<Transform>();
    if (nodeATransform == nullptr || nodeBTransform == nullptr)
        return false;

    const auto nodeAPosition = nodeATransform->GetPosition();
    const auto nodeBPosition = nodeBTransform->GetPosition();

    const auto nodeAScale = nodeATransform->GetScale();
    const auto nodeBScale = nodeBTransform->GetScale();

    const auto overlapX = (nodeAScale.X() + nodeBScale.X()) * 0.5f;
    const auto overlapY = (nodeAScale.Y() + nodeBScale.Y()) * 0.5f;

    return std::abs(nodeBPosition.X() - nodeAPosition.X()) <= overlapX &&
           std::abs(nodeBPosition.Y() - nodeAPosition.Y()) <= overlapY;
}

struct Ball final : Kek::World::Component
{
    Vector3 direction {1, 1, 0};
    float speed = 0.07f;

    void Tick() override
    {
        auto* transform = GetNode()->GetComponent<Transform>();
        const Vector3 position = transform->GetPosition();

        if (position.Y() <= -2.5f)
            direction = Vector3{direction.X(), 1, 0};
        else if (position.Y() >= 2.5f)
            direction = Vector3{direction.X(), -1, 0};

        transform->SetPosition(position + direction * speed);

        speed += 0.00001f;
    }
};

struct PlayerPaddleController final : Kek::World::Component
{
    Vector3 direction = Vector3{0, 0, 0};
    float speed = 0.1f;

    Node* ball = nullptr;

    void checkCollision() const
    {
        if (auto* ballComponent = ball->GetComponent<Ball>();
            overlaps(GetNode(), ball) && ballComponent != nullptr && ballComponent->direction.X() < 0.0f)
        {
            ballComponent->direction = Vector3{0.5f, ballComponent->direction.Y(), 0};
        }
    }

    void Tick() override
    {
        auto* transform = GetNode()->GetComponent<Transform>();
        const auto position = transform->GetPosition();

        if (Kek::Input::GetKey(Key::Up)) direction = Vector3{0, 1, 0};
        if (Kek::Input::GetKey(Key::Down)) direction = Vector3{0, -1, 0};

        Vector3 nextPosition = position + direction * speed;
        nextPosition = Vector3{nextPosition.X(), std::clamp(nextPosition.Y(), -2.0f, 2.0f), nextPosition.Z()};
        transform->SetPosition(nextPosition);

        direction = Vector3{0, 0, 0};

        if (ball != nullptr) checkCollision();
    }
};

struct EnemyPaddleAI final : Kek::World::Component
{
    Node* ball = nullptr;
    float speed = 0.06f;

    void checkCollision() const
    {
        if (auto* ballComponent = ball->GetComponent<Ball>();
            overlaps(GetNode(), ball) && ballComponent != nullptr && ballComponent->direction.X() > 0.0f)
        {
            ballComponent->direction = Vector3{-0.5f, ballComponent->direction.Y(), 0};
        }
    }

    void Tick() override
    {
        if (ball == nullptr) return;

        auto* transform = GetNode()->GetComponent<Transform>();

        const auto position = transform->GetPosition();
        const auto ballPosition = ball->GetComponent<Transform>()->GetPosition();

        const float distance = ballPosition.Y() - position.Y();
        const float movement = std::clamp(distance, -speed, speed);

        const auto nextPosition = Vector3{
            position.X(),
            std::clamp(position.Y() + movement, -2.0f, 2.0f),
            position.Z()
        };
        transform->SetPosition(nextPosition);

        if (ball != nullptr) checkCollision();
    }
};

int main()
{
    auto root = std::make_unique<Node>();

    auto playerPaddle = std::make_unique<Node>();
    playerPaddle->AttachComponent<Transform>(Vector3{-2.5f, 0, 0});
    playerPaddle->GetComponent<Transform>()->SetScale(Vector3{0.3f, 1, 0.3f});
    playerPaddle->AttachComponent<Components::MeshDisplay>(Mesh::Cube());

    auto* playerController = playerPaddle->AttachComponent<PlayerPaddleController>();
    auto ball = std::make_unique<Node>();
    Node* ballNode = ball.get();
    playerController->ball = ballNode;
    root->AddChild(std::move(playerPaddle));

    ball->AttachComponent<Transform>(Vector3{0, 0, 0});
    ball->GetComponent<Transform>()->SetScale(Vector3{0.3f, 0.3f, 0.3f});
    ball->AttachComponent<Components::MeshDisplay>(Mesh::Cube());
    ball->AttachComponent<Ball>();
    root->AddChild(std::move(ball));

    auto enemyPaddle = std::make_unique<Node>();
    enemyPaddle->AttachComponent<Transform>(Vector3{2.5f, 0, 0});
    enemyPaddle->GetComponent<Transform>()->SetScale(Vector3{0.3f, 1, 0.3f});
    enemyPaddle->AttachComponent<Components::MeshDisplay>(Mesh::Cube());
    auto* enemyAI = enemyPaddle->AttachComponent<EnemyPaddleAI>();
    enemyAI->ball = ballNode;
    root->AddChild(std::move(enemyPaddle));

    auto camera = std::make_unique<Node>();
    camera->AttachComponent<Transform>(Vector3{0, 0, 5});
    camera->GetComponent<Transform>()->SetRotation(Vector3{0, 0, 0});
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
