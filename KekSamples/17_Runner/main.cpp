//
// Created by vladk on 04.11.2026.
//


// A/D or Left/Right - change lane, Space/W/Up - jump, S/Down - slide

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "KekGraphics/Hud.hpp"
#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/Light.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekInput/Input.hpp"
#include "KekWorld/Node.hpp"
#include "KekWorld/Transform.hpp"

using namespace Kek::Graphics;
using Hud::Hidden;
using Kek::Input::Key;
using Kek::Math::Vector3;
using Kek::World::Node;
using Kek::World::Transform;

namespace
{
    constexpr float Dt = 1.0f / 60.0f;
    constexpr float LaneWidth = 1.6f;

    constexpr float StandHeight = 1.2f;
    constexpr float SlideHeight = 0.6f;

    constexpr int RowCount = 8;
    constexpr float RowSpacing = 10.0f;
    constexpr float TrackLength = RowCount * RowSpacing;
    constexpr float RecycleZ = 8.0f;

    constexpr int MarkerCount = 20;
    constexpr float MarkerSpacing = TrackLength / MarkerCount;


    // AABB overlap between two boxes given by center and size
    bool Overlaps(const Vector3& aPos, const Vector3& aSize, const Vector3& bPos, const Vector3& bSize)
    {
        return std::abs(aPos.X() - bPos.X()) <= (aSize.X() + bSize.X()) * 0.5f &&
               std::abs(aPos.Y() - bPos.Y()) <= (aSize.Y() + bSize.Y()) * 0.5f &&
               std::abs(aPos.Z() - bPos.Z()) <= (aSize.Z() + bSize.Z()) * 0.5f;
    }

    enum Kind { Low = 0, Tall = 1, Overhead = 2, Train = 3 };
    constexpr int KindCount = 4;

    Vector3 ObstacleSize(const int kind)
    {
        switch (kind)
        {
            case Low:      return Vector3{1.3f, 0.6f, 0.6f};  // jump over
            case Tall:     return Vector3{1.3f, 2.0f, 0.8f};  // dodge
            case Overhead: return Vector3{1.4f, 0.5f, 0.8f};  // slide under
            default:       return Vector3{1.4f, 2.2f, 8.0f};  // train: dodge
        }
    }

    float ObstacleY(const int kind)
    {
        switch (kind)
        {
            case Low: return 0.3f;
            case Tall: return 1.0f;
            case Overhead: return 1.15f; // bottom edge at 0.9
            default: return 1.1f;
        }
    }

    Vector3 ObstacleColor(const int kind)
    {
        switch (kind)
        {
            case Low: return Vector3{1.0f, 0.55f, 0.1f};
            case Tall: return Vector3{0.9f, 0.2f, 0.2f};
            case Overhead: return Vector3{0.7f, 0.3f, 0.9f};
            default: return Vector3{0.35f, 0.45f, 0.6f};
        }
    }

    // Creates a colored cube node, adds it to the root and returns its Transform
    Transform* AddCube(Node& root, const Vector3& position, const Vector3& scale, const Vector3& color)
    {
        Transform* transform = Hud::MakeCube(root, color, false, scale);
        transform->SetPosition(position);
        return transform;
    }
}

struct RunnerGame final : Kek::World::Component
{

    struct Obstacle
    {
        Transform* variants[KindCount]{}; // one cube per kind
        int kind = -1;                    // -1 = inactive
        int lane = 0;
    };

    struct Coin
    {
        Transform* transform = nullptr;
        Vector3 pos{0, Hidden, 0};
        bool active = false;
    };

    struct Row
    {
        float z = 0;
        Obstacle obstacles[3];
        Coin coins[5];
    };

    Transform* player = nullptr;
    Transform* camera = nullptr;
    std::vector<Transform*> markers;
    std::vector<Row> rows;

    std::mt19937 rng{std::random_device{}()};

    // game state
    int targetLane = 0;
    float playerX = 0;
    float jumpHeight = 0;
    float verticalSpeed = 0;
    float slideTimer = 0;
    float speed = 0;
    float distance = 0;
    int coins = 0;
    bool dead = false;
    float deadTimer = 0;

    static constexpr float StartSpeed = 10.0f;
    static constexpr float MaxSpeed = 24.0f;

    int RandomLane() { return std::uniform_int_distribution<int>(-1, 1)(rng); }
    float Chance() { return std::uniform_real_distribution<float>(0.0f, 1.0f)(rng); }


    void HideObstacle(Obstacle& o)
    {
        for (Transform* variant : o.variants)
            variant->SetPosition(Vector3{0, Hidden, 0});
        o.kind = -1;
    }

    // Fills a row with a fresh random layout: 1-2 obstacles (never all three lanes) and a coin
    void RollRow(Row& row)
    {
        for (Obstacle& o : row.obstacles) HideObstacle(o);

        const int laneA = RandomLane();
        const float r = Chance();
        row.obstacles[0].kind = r < 0.4f ? Low : (r < 0.7f ? Tall : Overhead);
        row.obstacles[0].lane = laneA;

        if (Chance() < 0.4f)
        {
            int laneB = RandomLane();
            if (laneB == laneA) laneB = laneA == 1 ? -1 : laneA + 1;
            row.obstacles[1].kind = Chance() < 0.5f ? Low : Overhead;
            row.obstacles[1].lane = laneB;
        }

        const float x = static_cast<float>(RandomLane()) * LaneWidth;
        for (int i = 0; i < 5; ++i)
        {
            Coin& c = row.coins[i];
            c.active = true;
            c.pos = Vector3{x, 0.7f, row.z - RowSpacing * 0.5f + (-4.0f + 2.0f * static_cast<float>(i))};
        }
    }

    void Reset()
    {
        targetLane = 0;
        playerX = 0;
        jumpHeight = 0;
        verticalSpeed = 0;
        slideTimer = 0;
        speed = StartSpeed;
        distance = 0;
        coins = 0;
        dead = false;
        deadTimer = 0;

        for (int i = 0; i < static_cast<int>(rows.size()); ++i)
        {
            rows[i].z = -25.0f - static_cast<float>(i) * RowSpacing;
            RollRow(rows[i]);
        }
    }

    void Start() override
    {
        Reset();
    }

    void HandleInput()
    {
        if ((Kek::Input::GetKeyPressed(Key::Left) || Kek::Input::GetKeyPressed(Key::A)) && targetLane > -1)
            --targetLane;
        if ((Kek::Input::GetKeyPressed(Key::Right) || Kek::Input::GetKeyPressed(Key::D)) && targetLane < 1)
            ++targetLane;

        const bool onGround = jumpHeight <= 0.0f && verticalSpeed <= 0.0f;

        if ((Kek::Input::GetKeyPressed(Key::Space) || Kek::Input::GetKeyPressed(Key::W) || Kek::Input::GetKeyPressed(Key::Up)) && onGround)
        {
            verticalSpeed = 7.0f;
            slideTimer = 0;
        }

        if (Kek::Input::GetKeyPressed(Key::S) || Kek::Input::GetKeyPressed(Key::Down))
        {
            if (onGround) slideTimer = 0.6f;
            else verticalSpeed = -14.0f; // fast fall
        }
    }

    void UpdatePlayer()
    {
        playerX += (static_cast<float>(targetLane) * LaneWidth - playerX) * 0.25f;

        verticalSpeed -= 20.0f * Dt;
        jumpHeight += verticalSpeed * Dt;
        if (jumpHeight <= 0.0f)
        {
            jumpHeight = 0.0f;
            verticalSpeed = 0.0f;
        }

        slideTimer = std::max(0.0f, slideTimer - Dt);
    }

    Vector3 PlayerSize() const
    {
        return Vector3{0.8f, slideTimer > 0.0f ? SlideHeight : StandHeight, 0.6f};
    }

    Vector3 PlayerPosition() const
    {
        return Vector3{playerX, jumpHeight + PlayerSize().Y() * 0.5f, 0};
    }

    [[nodiscard]] int Score() const { return static_cast<int>(distance) + coins * 10; }

    void UpdateWorld()
    {
        const float step = speed * Dt;
        distance += step;
        speed = std::min(MaxSpeed, speed + 0.3f * Dt);

        for (Transform* marker : markers)
        {
            Vector3 p = marker->GetPosition();
            float z = p.Z() + step;
            if (z > RecycleZ) z -= TrackLength;
            marker->SetPosition(Vector3{p.X(), p.Y(), z});
        }

        const Vector3 playerPos = PlayerPosition();
        const Vector3 playerSize = PlayerSize();
        const Vector3 coinSize{0.5f, 0.5f, 0.5f};

        for (Row& row : rows)
        {
            row.z += step;
            if (row.z > RecycleZ)
            {
                row.z -= TrackLength;
                RollRow(row);
            }

            for (Obstacle& o : row.obstacles)
            {
                if (o.kind < 0) continue;

                const Vector3 pos{static_cast<float>(o.lane) * LaneWidth, ObstacleY(o.kind), row.z};
                o.variants[o.kind]->SetPosition(pos);

                if (Overlaps(playerPos, playerSize, pos, ObstacleSize(o.kind)))
                    Die();
            }

            // The coin sits halfway between this row and the next one
            for (Coin& c : row.coins)
            {
                if (!c.active) continue;

                c.pos = Vector3{c.pos.X(), c.pos.Y(), c.pos.Z() + step};
                c.transform->SetPosition(c.pos);
                c.transform->Rotate(Vector3{0, 0.12f, 0});

                if (Overlaps(playerPos, playerSize, c.pos, coinSize))
                {
                    c.active = false;
                    c.transform->SetPosition(Vector3{0, Hidden, 0});
                    ++coins;
                }
            }
        }
    }

    void Die()
    {
        if (dead) return;
        dead = true;
        deadTimer = 1.5f;
        std::cout << "Crashed! Distance: " << static_cast<int>(distance)
                  << " m, coins: " << coins
                  << ", score: " << Score() << "\n";
    }

    void Tick() override
    {
        if (dead)
        {
            deadTimer -= Dt;
            if (deadTimer <= 0.0f) Reset();
            return;
        }

        HandleInput();
        UpdatePlayer();
        UpdateWorld();

        player->SetPosition(PlayerPosition());
        player->SetScale(PlayerSize());

        const Vector3 cam = camera->GetPosition();
        camera->SetPosition(Vector3{playerX * 0.6f, cam.Y(), cam.Z()});
    }
};

int main()
{
    auto root = std::make_unique<Node>();

    // Camera behind and above the runner, looking slightly down
    auto cameraNode = std::make_unique<Node>();
    auto* cameraTransform = cameraNode->AttachComponent<Transform>(Vector3{0, 3.2f, 6.0f});
    cameraTransform->SetRotation(Vector3{-0.35f, 0, 0});
    cameraNode->AttachComponent<Components::Camera>(1.0471976f, 0.1f, 200.0f);
    root->AddChild(std::move(cameraNode));

    auto lightNode = std::make_unique<Node>();
    lightNode->AttachComponent<Transform>(Vector3{0, 6.0f, -3.0f});
    lightNode->AttachComponent<Components::Light>(Vector3{1, 1, 1}, 1.2f);
    root->AddChild(std::move(lightNode));

    // Floor and lane lines
    AddCube(*root, Vector3{0, -0.1f, -TrackLength * 0.5f}, Vector3{5.2f, 0.2f, TrackLength + 40.0f}, Vector3{0.35f, 0.35f, 0.4f});

    auto game = std::make_unique<Node>();
    auto* runner = game->AttachComponent<RunnerGame>();

    for (const float x : {-LaneWidth * 0.5f, LaneWidth * 0.5f})
    {
        for (int i = 0; i < MarkerCount; ++i)
        {
            runner->markers.push_back(AddCube(*root, Vector3{x, 0.01f, 5.0f - static_cast<float>(i) * MarkerSpacing},
                                              Vector3{0.06f, 0.02f, 1.5f}, Vector3{0.9f, 0.9f, 0.9f}));
        }
    }

    // Player
    runner->player = AddCube(*root, Vector3{0, 0.6f, 0}, Vector3{0.8f, StandHeight, 0.6f}, Vector3{0.3f, 0.8f, 1.0f});

    // Object pool: rows of obstacles and coins that get recycled as they pass the camera
    runner->rows.resize(RowCount);
    for (auto& row : runner->rows)
    {
        for (auto& o : row.obstacles)
        {
            for (int k = 0; k < KindCount; ++k)
                o.variants[k] = AddCube(*root, Vector3{0, Hidden, 0}, ObstacleSize(k), ObstacleColor(k));
        }
    }

    runner->camera = cameraTransform;
    root->AddChild(std::move(game));

    Kek::Application app(Kek::Platform::WindowSettings
    {
        480,
        800,
        "KekLib - 17 Runner"
    },
    std::move(root));
    app.SetTickRate(60);
    app.Run();
    return 0;
}