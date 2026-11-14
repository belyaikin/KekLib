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
#include <exception>

#include "KekGraphics/Hud.hpp"
#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/Light.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekInput/Input.hpp"
#include "KekWorld/Node.hpp"
#include "KekWorld/Transform.hpp"
#include "KekGraphics/RenderRegistry.hpp"
#include "KekMath/Matrix4.hpp"

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
    constexpr float RowSpacing = 16.0f;
    constexpr float TrackLength = RowCount * RowSpacing;
    constexpr float RecycleZ = 10.0f;

    constexpr int MarkerCount = 20;
    constexpr float MarkerSpacing = TrackLength / MarkerCount;

    constexpr float DecorLength = 140.0f;
    constexpr float DecorRecycleZ = 14.0f;
    constexpr int BuildingsPerSide = 14;


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

    Vector3 HudPos(const float x, const float y) { return Vector3{x, y, Hud::Depth}; }

    constexpr const char* TopicTitle = "KEKLIB";
    constexpr const char* TeamName = "TEAM NAME";

    constexpr float FlySpeed = 0.2f;
    constexpr float FlyTurnSpeed = 0.025f;

    struct HelpLine { const char* text; float pixel; float x; float y; };

    constexpr HelpLine HelpLines[] = {
        {"HELP",               0.050f, -0.375f,  1.35f},
        {"A D  LANE",          0.025f, -0.95f,   0.95f},
        {"SPACE  JUMP",        0.025f, -0.95f,   0.75f},
        {"S  SLIDE",           0.025f, -0.95f,   0.55f},
        {"P  PAUSE",           0.025f, -0.95f,   0.35f},
        {"L  LIGHT ON OFF",    0.025f, -0.95f,   0.15f},
        {"Q E R F Z X  LIGHT", 0.025f, -0.95f,  -0.05f},
        {"C  FLY CAMERA",      0.025f, -0.95f,  -0.25f},
        {"WASD ARROWS  FLY",   0.025f, -0.95f,  -0.45f},
        {"H  CLOSE",           0.025f, -0.95f,  -0.65f},
    };
}

struct RunnerGame final : Kek::World::Component
{

    enum class State { Menu, Playing, Paused, GameOver };

    State state = State::Menu;
    float stateTimer = 0;
    int best = 0;
    bool newRecord = false;

    static constexpr int MaxLives = 3;

    int lives = MaxLives;
    float invulnTimer = 0;
    float shakeTimer = 0;
    float camX = 0;

    static constexpr float MagnetDuration = 8.0f;

    float magnetTimer = 0;
    Transform* magnetAura = nullptr;
    Transform* magnetBar = nullptr;

    static constexpr float ShieldDuration = 6.0f;

    float shieldTimer = 0;
    Transform* shieldAura = nullptr;
    Transform* shieldBar = nullptr;

    int lastSafe = 0;
    bool lastNeedsAction = false;

    int RandInt(const int lo, const int hi) { return std::uniform_int_distribution<int>(lo, hi)(rng); }

    std::vector<Transform*> hearts;

    std::unique_ptr<Hud::Digits> bigScore, bigBest;
    std::unique_ptr<Hud::Text> title, record, prompt, labelScore, labelBest;

    static bool Pressed(const Key key) { return Kek::Input::GetKeyPressed(key); }
    static bool Confirm() { return Pressed(Key::Space) || Pressed(Key::Enter); }
    static bool PausePressed() { return Pressed(Key::P) || Pressed(Key::Escape); }

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
        bool pulled = false;
    };

    struct Row
    {
        float z = 0;
        Obstacle obstacles[3];
        Coin coins[5];
        Transform* pickups[2]{};
        int pickupKind = -1;
        Vector3 pickupPos{0, Hidden, 0};
    };

    struct Building
    {
        Transform* transform = nullptr;
        float z = 0;
        float side = 1;
        float width = 4;
        float height = 8;
    };

    std::vector<Building> buildings;

    void Reroll(Building& b)
    {
        b.width = 3.0f + Chance() * 3.0f;
        b.height = 5.0f + Chance() * 11.0f;
        b.transform->SetScale(Vector3{b.width, b.height, 8.0f});
    }

    void PlaceBuilding(const Building& b)
    {
        b.transform->SetPosition(Vector3{b.side * (6.0f + b.width * 0.5f), b.height * 0.5f - 0.3f, b.z});
    }

    void ScrollBuildings(const float step)
    {
        for (Building& b : buildings)
        {
            b.z += step;
            if (b.z > DecorRecycleZ)
            {
                b.z -= DecorLength;
                Reroll(b);
            }
            PlaceBuilding(b);
        }
    }

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
    void RollRow(Row& row, const bool empty)
    {
        ClearRow(row);

        const int safe = std::clamp(lastSafe + RandInt(-1, 1), -1, 1);
        int safeKind = -1; // obstacle placed in the safe lane that needs a jump or a slide
        int slot = 0;

        if (!empty)
        {
            for (int lane = -1; lane <= 1; ++lane)
            {
                if (lane == safe) continue;

                const float r = Chance();
                if (r < 0.15f) continue; // empty lane

                row.obstacles[slot].kind = r < 0.40f ? Train : (r < 0.60f ? Tall : (r < 0.80f ? Low : Overhead));
                row.obstacles[slot].lane = lane;
                ++slot;
            }

            // Sometimes the safe lane has a jump or slide obstacle, but never in two rows in a row.
            if (!lastNeedsAction && Chance() < 0.3f)
            {
                safeKind = Chance() < 0.5f ? Low : Overhead;
                row.obstacles[slot].kind = safeKind;
                row.obstacles[slot].lane = safe;
            }
        }

        lastSafe = safe;
        lastNeedsAction = safeKind >= 0;

        // A line of coins in the safe lane (an arc over low barriers).
        const float x = static_cast<float>(safe) * LaneWidth;
        for (int i = 0; i < 5; ++i)
        {
            if (safeKind == Overhead && i == 2) continue;

            const float offset = -4.0f + 2.0f * static_cast<float>(i);
            float y = 0.7f;
            if (safeKind == Low)
            {
                const float k = offset / 4.0f;
                y = 0.7f + 1.0f * (1.0f - k * k);
            }

            Coin& c = row.coins[i];
            c.active = true;
            c.pulled = false;
            c.pos = Vector3{x, y, row.z + offset};
        }

        // Power-up between this row and the next one.
        if (!empty && Chance() < 0.12f)
        {
            row.pickupKind = RandInt(0, 1); // 0 = magnet, 1 = shield
            row.pickupPos = Vector3{static_cast<float>(RandInt(-1, 1)) * LaneWidth, 0.8f, row.z - RowSpacing * 0.5f};
        }
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


    std::unique_ptr<Hud::Digits> scoreDigits, coinDigits;
    Transform* coinIcon = nullptr;

    static void SetVisible(Transform* transform, const Vector3& pos, const bool on)
    {
        transform->SetPosition(on ? pos : Vector3{pos.X(), Hidden, pos.Z()});
    }

    void UpdateHud()
    {
        scoreDigits->Show(Score());
        coinDigits->Show(coins);

        SetVisible(coinIcon, HudPos(0.32f, 1.5f), true);
        coinIcon->Rotate(Vector3{0, 0.08f, 0});

        for (std::size_t i = 0; i < hearts.size(); ++i)
            SetVisible(hearts[i], HudPos(-0.9f + 0.2f * static_cast<float>(i), 1.27f), static_cast<int>(i) < lives);

        const float m = std::clamp(magnetTimer / MagnetDuration, 0.0f, 1.0f);
        magnetBar->SetScale(Vector3{std::max(0.001f, 0.8f * m), 0.06f, 0.02f});
        SetVisible(magnetBar, HudPos(-0.95f + 0.4f * m, 1.1f), magnetTimer > 0.0f);

        const float s = std::clamp(shieldTimer / ShieldDuration, 0.0f, 1.0f);
        shieldBar->SetScale(Vector3{std::max(0.001f, 0.8f * s), 0.06f, 0.02f});
        SetVisible(shieldBar, HudPos(-0.95f + 0.4f * s, 1.0f), shieldTimer > 0.0f);
    }

    Transform* body = nullptr;
    Transform* head = nullptr;
    Transform* legL = nullptr;
    Transform* legR = nullptr;
    Transform* shadow = nullptr;

    float runPhase = 0;

    void AnimatePlayer()
    {
        const float x = playerX;
        const float feet = jumpHeight;
        const bool sliding = slideTimer > 0.0f;
        const bool air = feet > 0.001f;
        const bool dead = state == State::GameOver;
        const bool blink = state == State::Playing && invulnTimer > 0.0f && static_cast<int>(invulnTimer * 14.0f) % 2 == 0;

        runPhase += (4.0f + speed) * Dt;

        const float swing = std::sin(runPhase);
        const float bob = (air || sliding || dead) ? 0.0f : std::abs(swing) * 0.05f;
        const float tilt = std::clamp((x - static_cast<float>(targetLane) * LaneWidth) * 0.25f, -0.35f, 0.35f);

        auto put = [&](Transform* t, const Vector3& pos, const Vector3& scale)
        {
            t->SetScale(scale);
            t->SetPosition(blink ? Vector3{pos.X(), Hidden, pos.Z()} : pos);
        };

        if (dead)
        {
            put(body, {x, 0.2f, 0}, {1.0f, 0.4f, 0.5f});
            put(head, {x + 0.7f, 0.22f, 0}, {0.38f, 0.38f, 0.38f});
            put(legL, {x - 0.65f, 0.15f, 0.15f}, {0.4f, 0.25f, 0.28f});
            put(legR, {x - 0.65f, 0.15f, -0.15f}, {0.4f, 0.25f, 0.28f});
            body->SetRotation(Vector3{0, 0, 0});
        }
        else if (sliding)
        {
            put(body, {x, feet + 0.22f, 0}, {0.8f, 0.4f, 0.9f});
            put(head, {x, feet + 0.45f, -0.4f}, {0.34f, 0.34f, 0.34f});
            put(legL, {x - 0.2f, feet + 0.13f, 0.6f}, {0.26f, 0.26f, 0.5f});
            put(legR, {x + 0.2f, feet + 0.13f, 0.6f}, {0.26f, 0.26f, 0.5f});
            body->SetRotation(Vector3{0, 0, 0});
        }
        else
        {
            const float legZ = air ? 0.25f : swing * 0.3f;
            put(legL, {x - 0.2f, feet + 0.22f, legZ}, {0.3f, 0.44f, 0.3f});
            put(legR, {x + 0.2f, feet + 0.22f, -legZ}, {0.3f, 0.44f, 0.3f});
            put(body, {x, feet + 0.69f + bob, 0}, {0.8f, 0.5f, 0.5f});
            put(head, {x, feet + 1.02f + bob, 0}, {0.38f, 0.38f, 0.38f});
            body->SetRotation(Vector3{0, 0, tilt});
        }

        const float k = 1.0f / (1.0f + feet * 0.6f);
        shadow->SetScale(Vector3{0.9f * k, 0.02f, 0.7f * k});
        shadow->SetPosition(Vector3{x, 0.03f, 0});

        const bool playing = state == State::Playing;

        SetVisible(shieldAura, Vector3{x, 0.09f, 0}, playing && shieldTimer > 0.0f);
        shieldAura->SetScale(Vector3{1.4f, 0.04f, 1.4f});
        shieldAura->SetRotation(Vector3{0, runPhase * 0.2f, 0});

        SetVisible(magnetAura, Vector3{x, 0.05f, 0}, playing && magnetTimer > 0.0f);
        magnetAura->SetScale(Vector3{2.0f, 0.04f, 2.0f});
        magnetAura->SetRotation(Vector3{0, -runPhase * 0.2f, 0});
    }

    void UpdateCamera()
    {
        camX += (playerX * 0.6f - camX) * 0.1f;

        float shakeX = 0, shakeY = 0;
        if (shakeTimer > 0.0f)
        {
            shakeX = (Chance() - 0.5f) * 0.8f * shakeTimer;
            shakeY = (Chance() - 0.5f) * 0.8f * shakeTimer;
            shakeTimer = std::max(0.0f, shakeTimer - Dt);
        }

        camera->SetPosition(Vector3{camX + shakeX, 3.2f + shakeY, 6.0f});
    }

    void ScrollDecor(const float step)
    {
        for (Transform* marker : markers)
        {
            const Vector3 p = marker->GetPosition();
            float z = p.Z() + step;
            if (z > RecycleZ) z -= TrackLength;
            marker->SetPosition(Vector3{p.X(), p.Y(), z});
        }

        ScrollBuildings(step);
    }

    void UpdateRows(const float step)
    {
        const Vector3 playerPos = PlayerPosition();
        const Vector3 playerSize = PlayerSize();
        const Vector3 hitSize{playerSize.X() * 0.75f, playerSize.Y() - 0.1f, playerSize.Z() * 0.8f};
        const Vector3 coinSize{0.5f, 0.5f, 0.5f};
        const Vector3 pickupSize{0.7f, 0.7f, 0.7f};

        for (Row& row : rows)
        {
            row.z += step;
            if (row.z > RecycleZ)
            {
                row.z -= TrackLength;
                RollRow(row, false);
            }

            for (Obstacle& o : row.obstacles)
            {
                if (o.kind < 0) continue;

                const Vector3 pos{static_cast<float>(o.lane) * LaneWidth, ObstacleY(o.kind), row.z};
                o.variants[o.kind]->SetPosition(pos);

                if (invulnTimer <= 0.0f && state == State::Playing &&
                    Overlaps(playerPos, hitSize, pos, ObstacleSize(o.kind)))
                    OnHit();
            }

            for (Coin& c : row.coins)
            {
                if (!c.active) continue;

                if (magnetTimer > 0.0f && !c.pulled &&
                    std::abs(c.pos.Z()) < 9.0f && std::abs(c.pos.X() - playerX) < LaneWidth * 1.6f)
                    c.pulled = true;

                if (c.pulled) c.pos = c.pos + (playerPos - c.pos) * 0.3f;
                else c.pos = Vector3{c.pos.X(), c.pos.Y(), c.pos.Z() + step};

                c.transform->SetPosition(c.pos);
                c.transform->Rotate(Vector3{0, 0.12f, 0});

                if (Overlaps(playerPos, playerSize, c.pos, coinSize))
                {
                    c.active = false;
                    c.pulled = false;
                    c.transform->SetPosition(Vector3{0, Hidden, 0});
                    ++coins;
                }
            }

            if (row.pickupKind >= 0)
            {
                row.pickupPos = Vector3{row.pickupPos.X(), row.pickupPos.Y(), row.pickupPos.Z() + step};

                Transform* pickup = row.pickups[row.pickupKind];
                pickup->SetPosition(row.pickupPos);
                pickup->Rotate(Vector3{0.05f, 0.1f, 0});

                if (Overlaps(playerPos, playerSize, row.pickupPos, pickupSize))
                {
                    if (row.pickupKind == 0) magnetTimer = MagnetDuration;
                    else shieldTimer = ShieldDuration;

                    pickup->SetPosition(Vector3{0, Hidden, 0});
                    row.pickupKind = -1;
                }
            }
        }
    }

    void HideOverlay()
    {
        title->Hide();
        record->Hide();
        prompt->Hide();
        labelScore->Hide();
        labelBest->Hide();
        bigScore->Hide();
        bigBest->Hide();
        topicText->Hide();
        teamText->Hide();
    }

    void ShowOverlay()
    {
        HideOverlay();

        switch (state)
        {
            case State::Menu:
                title->Show("RUNNER", 0.0f, 0.8f, 0.06f);
                topicText->Show(TopicTitle, 0.0f, 0.45f, 0.03f);
                teamText->Show(TeamName, 0.0f, 0.25f, 0.025f);
                labelBest->Show("BEST", -0.65f, -0.4f, 0.03f);
                bigBest->Show(best);
                break;
            case State::Paused:
                title->Show("PAUSED", 0.0f, 0.4f, 0.06f);
                break;
            case State::GameOver:
                title->Show("GAME OVER", 0.0f, 0.9f, 0.045f);
                if (newRecord) record->Show("NEW BEST", 0.0f, 0.5f, 0.035f);
                labelScore->Show("SCORE", -0.65f, 0.0f, 0.03f);
                bigScore->Show(Score());
                labelBest->Show("BEST", -0.65f, -0.4f, 0.03f);
                bigBest->Show(best);
                break;
            case State::Playing:
                break;
        }
    }

    void UpdatePrompt()
    {
        const bool on = std::fmod(stateTimer, 1.0f) < 0.65f;
        if (on) prompt->Show(state == State::Paused ? "PRESS P" : "PRESS SPACE", 0.0f, -1.0f, 0.03f);
        else prompt->Hide();
    }

    void HideHud()
    {
        scoreDigits->Hide();
        coinDigits->Hide();
        SetVisible(coinIcon, HudPos(0.32f, 1.5f), false);

        for (std::size_t i = 0; i < hearts.size(); ++i)
            SetVisible(hearts[i], HudPos(-0.9f + 0.2f * static_cast<float>(i), 1.27f), false);

        SetVisible(magnetBar, HudPos(-0.95f, 1.1f), false);

        SetVisible(shieldBar, HudPos(-0.95f, 1.0f), false);
    }

    void EnterMenu()
    {
        state = State::Menu;
        stateTimer = 0;
        invulnTimer = magnetTimer = shieldTimer = 0;
        speed = 6.0f;
        HideHud();
        ShowOverlay();
    }

    void StartRun()
    {
        state = State::Playing;
        stateTimer = 0;

        targetLane = 0;
        playerX = 0;
        jumpHeight = 0;
        verticalSpeed = 0;
        slideTimer = 0;

        speed = StartSpeed;
        distance = 0;
        coins = 0;

        lives = MaxLives;
        invulnTimer = magnetTimer = shieldTimer = shakeTimer = 0;

        lastSafe = 0;
        lastNeedsAction = false;

        for (int i = 0; i < static_cast<int>(rows.size()); ++i)
        {
            rows[i].z = -30.0f - static_cast<float>(i) * RowSpacing;
            RollRow(rows[i], i < 2);
        }

        HideOverlay();
    }

    void EndRun()
    {
        state = State::GameOver;
        stateTimer = 0;
        invulnTimer = 0;

        newRecord = Score() > best;
        if (newRecord)
        {
            best = Score();
            SaveBest();
        }

        std::cout << "Crashed! Distance: " << static_cast<int>(distance) << " m, coins: " << coins
                  << ", score: " << Score() << ", best: " << best << std::endl;

        HideHud();
        ShowOverlay();
    }

    void TickMenu()
    {
        speed = 6.0f;
        ScrollDecor(speed * Dt);
        AnimatePlayer();
        UpdateCamera();
        UpdatePrompt();

        if (Confirm()) StartRun();
    }

    void TickPlaying()
    {
        if (PausePressed())
        {
            state = State::Paused;
            stateTimer = 0;
            ShowOverlay();
            return;
        }

        HandleInput();
        UpdatePlayer();

        const float step = speed * Dt;
        distance += step;
        speed = std::min(MaxSpeed, speed + 0.3f * Dt);

        ScrollDecor(step);
        UpdateRows(step);

        invulnTimer = std::max(0.0f, invulnTimer - Dt);
        magnetTimer = std::max(0.0f, magnetTimer - Dt);
        shieldTimer = std::max(0.0f, shieldTimer - Dt);

        if (state == State::Playing) UpdateHud();

        AnimatePlayer();
        UpdateCamera();
    }

    void TickPaused()
    {
        UpdatePrompt();

        if (PausePressed() || Confirm())
        {
            state = State::Playing;
            stateTimer = 0;
            HideOverlay();
        }
    }

    void TickGameOver()
    {
        AnimatePlayer();
        UpdateCamera();
        UpdatePrompt();

        if (stateTimer > 0.6f && Confirm()) StartRun();
    }

    void Start() override
    {
        LoadBest();
        for (Building& b : buildings)
        {
            Reroll(b);
            PlaceBuilding(b);
        }

        std::cout << "A/D or arrows - lane, Space/W/Up - jump, S/Down - slide, P/Esc - pause" << std::endl;
        EnterMenu();
    }

    void ClearRow(Row& row)
    {
        for (Obstacle& o : row.obstacles) HideObstacle(o);

        for (Coin& c : row.coins)
        {
            c.active = false;
            c.pulled = false;
            c.transform->SetPosition(Vector3{0, Hidden, 0});
        }

        for (Transform* pickup : row.pickups) pickup->SetPosition(Vector3{0, Hidden, 0});
        row.pickupKind = -1;
    }

    void Tick() override
    {
        stateTimer += Dt;

        HandleDebugKeys();

        if (showHelp) return;                         // help screen freezes the game
        if (flyCam) { UpdateFlyCamera(); return; }    // fly camera freezes gameplay

        switch (state)
        {
            case State::Menu: TickMenu(); break;
            case State::Playing: TickPlaying(); break;
            case State::Paused: TickPaused(); break;
            case State::GameOver: TickGameOver(); break;
        }
    }

    static constexpr const char* BestFile = "runner_best.txt";

    void LoadBest()
    {
        std::ifstream file(BestFile);
        int value = 0;
        if (file >> value) best = value;
    }

    void SaveBest() const
    {
        std::ofstream file(BestFile);
        file << best;
    }

    void OnHit()
    {
        if (shieldTimer > 0.0f) // the shield absorbs one hit
        {
            shieldTimer = 0;
            invulnTimer = 1.5f;
            shakeTimer = 0.2f;
            return;
        }

        --lives;
        invulnTimer = 2.0f;
        shakeTimer = 0.4f;
        speed = std::max(StartSpeed, speed * 0.75f);

        if (lives <= 0) EndRun();
    }

    Transform* light = nullptr;
    Transform* lightMarker = nullptr;

    bool flyCam = false;
    Vector3 savedCamPos{0, 3.2f, 6.0f};
    Vector3 savedCamRot{-0.35f, 0, 0};

    bool showHelp = false;
    Transform* helpPanel = nullptr;
    std::vector<std::unique_ptr<Hud::Text>> helpTexts;
    std::unique_ptr<Hud::Text> lightStatus, camStatus, helpHint, topicText, teamText;

    static void ShowLeft(Hud::Text& text, const std::string& s, const float left, const float y, const float pixel)
    {
        const float width = (static_cast<float>(s.size()) * 4.0f - 1.0f) * pixel;
        text.Show(s, left + width * 0.5f, y, pixel);
    }

    void UpdateStatus()
    {
        lightStatus->Show(RenderRegistry::lightingEnabled ? "LIGHT ON" : "LIGHT OFF", -0.62f, -1.5f, 0.02f);
        camStatus->Show(flyCam ? "CAM FLY" : "CAM GAME", 0.0f, -1.5f, 0.02f);
        helpHint->Show("H HELP", 0.7f, -1.5f, 0.02f);
    }

    void SetHelp(const bool on)
    {
        showHelp = on;
        const Vector3 panelPos{0, 0, Hud::Depth - 0.05f};

        if (on)
        {
            HideOverlay();
            HideHud();
            SetVisible(helpPanel, panelPos, true);

            for (std::size_t i = 0; i < helpTexts.size(); ++i)
                ShowLeft(*helpTexts[i], HelpLines[i].text, HelpLines[i].x, HelpLines[i].y, HelpLines[i].pixel);

            topicText->Show(TopicTitle, 0.0f, -0.95f, 0.03f);
            teamText->Show(TeamName, 0.0f, -1.2f, 0.025f);
        }
        else
        {
            SetVisible(helpPanel, panelPos, false);
            for (auto& text : helpTexts) text->Hide();
            topicText->Hide();
            teamText->Hide();

            if (state != State::Playing) ShowOverlay();
        }
    }

    void UpdateLight()
    {
        Vector3 move{0, 0, 0};
        if (Kek::Input::GetKey(Key::Q)) move += Vector3{-1, 0, 0};
        if (Kek::Input::GetKey(Key::E)) move += Vector3{1, 0, 0};
        if (Kek::Input::GetKey(Key::R)) move += Vector3{0, 1, 0};
        if (Kek::Input::GetKey(Key::F)) move += Vector3{0, -1, 0};
        if (Kek::Input::GetKey(Key::Z)) move += Vector3{0, 0, -1};
        if (Kek::Input::GetKey(Key::X)) move += Vector3{0, 0, 1};

        Vector3 p = light->GetPosition() + move * 0.2f;
        p = Vector3{std::clamp(p.X(), -12.0f, 12.0f), std::clamp(p.Y(), 1.0f, 16.0f), std::clamp(p.Z(), -30.0f, 12.0f)};

        light->SetPosition(p);
        lightMarker->SetPosition(p); // small unlit cube so you can see where the light is
    }

    void ToggleFlyCam()
    {
        flyCam = !flyCam;

        if (flyCam)
        {
            savedCamPos = camera->GetPosition();
            savedCamRot = camera->GetRotation();
        }
        else
        {
            camera->SetRotation(savedCamRot);
            camera->SetPosition(savedCamPos);
        }
    }

    // W/S forward/back, A/D strafe, Space/LeftShift up/down, arrows look around.
    void UpdateFlyCamera()
    {
        Vector3 rotation = camera->GetRotation();
        if (Kek::Input::GetKey(Key::Left)) rotation += Vector3{0, FlyTurnSpeed, 0};
        if (Kek::Input::GetKey(Key::Right)) rotation += Vector3{0, -FlyTurnSpeed, 0};
        if (Kek::Input::GetKey(Key::Up)) rotation += Vector3{FlyTurnSpeed, 0, 0};
        if (Kek::Input::GetKey(Key::Down)) rotation += Vector3{-FlyTurnSpeed, 0, 0};

        rotation = Vector3{std::clamp(rotation.X(), -1.5f, 1.5f), rotation.Y(), rotation.Z()};
        camera->SetRotation(rotation);

        const Kek::Math::Matrix4 orientation = Kek::Math::Matrix4::Rotation(rotation);
        const Vector3 forward = orientation.TransformPoint(Vector3{0, 0, -1});
        const Vector3 right = orientation.TransformPoint(Vector3{1, 0, 0});

        Vector3 move{0, 0, 0};
        if (Kek::Input::GetKey(Key::W)) move += forward;
        if (Kek::Input::GetKey(Key::S)) move += -forward;
        if (Kek::Input::GetKey(Key::D)) move += right;
        if (Kek::Input::GetKey(Key::A)) move += -right;
        if (Kek::Input::GetKey(Key::Space)) move += Vector3::up;
        if (Kek::Input::GetKey(Key::LeftShift)) move += Vector3::down;

        camera->Move(move * FlySpeed);
    }

    void HandleDebugKeys()
    {
        if (Pressed(Key::L)) RenderRegistry::lightingEnabled = !RenderRegistry::lightingEnabled;
        if (Pressed(Key::C)) ToggleFlyCam();
        if (Pressed(Key::H) || Pressed(Key::F1)) SetHelp(!showHelp);
        else if (showHelp && Pressed(Key::Escape)) SetHelp(false);

        UpdateLight();
        UpdateStatus();
    }
};

int RunGame()
{
    auto root = std::make_unique<Node>();

    // Camera behind and above the runner, looking slightly down
    auto cameraNode = std::make_unique<Node>();
    auto* cameraTransform = cameraNode->AttachComponent<Transform>(Vector3{0, 3.2f, 6.0f});
    cameraTransform->SetRotation(Vector3{-0.35f, 0, 0});
    cameraNode->AttachComponent<Components::Camera>(1.0471976f, 0.1f, 200.0f);
    Node& cameraRoot = *cameraNode;
    root->AddChild(std::move(cameraNode));

    auto lightNode = std::make_unique<Node>();
    auto* lightTransform = lightNode->AttachComponent<Transform>(Vector3{0, 6.0f, -3.0f});
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

    // Ground strips and buildings on both sides
    std::mt19937 colorRng{std::random_device{}()};
    const Vector3 palette[6] = {
        {0.30f, 0.32f, 0.45f}, {0.45f, 0.30f, 0.32f}, {0.28f, 0.42f, 0.40f},
        {0.50f, 0.45f, 0.30f}, {0.35f, 0.30f, 0.50f}, {0.40f, 0.40f, 0.42f},
    };

    for (const float side : {-1.0f, 1.0f})
    {
        AddCube(*root, Vector3{side * 4.3f, -0.2f, -TrackLength * 0.5f}, Vector3{3.4f, 0.2f, TrackLength + 40.0f}, Vector3{0.22f, 0.25f, 0.22f});

        for (int i = 0; i < BuildingsPerSide; ++i)
        {
            const int colorIndex = std::uniform_int_distribution<int>(0, 5)(colorRng);

            RunnerGame::Building b;
            b.transform = AddCube(*root, Vector3{0, Hidden, 0}, Vector3{1, 1, 1}, palette[colorIndex]);
            b.side = side;
            b.z = 10.0f - static_cast<float>(i) * (DecorLength / static_cast<float>(BuildingsPerSide));
            runner->buildings.push_back(b);
        }
    }

    // Player
    runner->legL = Hud::MakeCube(*root, Vector3{0.15f, 0.2f, 0.5f}, false);
    runner->legR = Hud::MakeCube(*root, Vector3{0.15f, 0.2f, 0.5f}, false);
    runner->body = Hud::MakeCube(*root, Vector3{0.25f, 0.6f, 1.0f}, false);
    runner->head = Hud::MakeCube(*root, Vector3{1.0f, 0.8f, 0.65f}, false);
    runner->shadow = Hud::MakeCube(*root, Vector3{0.05f, 0.05f, 0.07f}, true);
    runner->magnetAura = Hud::MakeCube(*root, Vector3{0.3f, 0.9f, 1.0f}, true);
    runner->shieldAura = Hud::MakeCube(*root, Vector3{0.3f, 1.0f, 0.5f}, true);

    // Object pool: rows of obstacles and coins that get recycled as they pass the camera
    runner->rows.resize(RowCount);
    for (auto& row : runner->rows)
    {
        for (auto& o : row.obstacles)
        {
            for (int k = 0; k < KindCount; ++k)
                o.variants[k] = AddCube(*root, Vector3{0, Hidden, 0}, ObstacleSize(k), ObstacleColor(k));
        }

        for (auto& c : row.coins)
            c.transform = Hud::MakeCube(*root, Vector3{1.0f, 0.85f, 0.1f}, true, Vector3{0.4f, 0.4f, 0.1f});

        row.pickups[0] = Hud::MakeCube(*root, Vector3{0.3f, 0.9f, 1.0f}, true, Vector3{0.5f, 0.5f, 0.5f});
        row.pickups[1] = Hud::MakeCube(*root, Vector3{0.3f, 1.0f, 0.4f}, true, Vector3{0.5f, 0.5f, 0.5f});
    }

    const Vector3 white{1, 1, 1};
    const Vector3 yellow{1.0f, 0.85f, 0.2f};

    runner->scoreDigits = std::make_unique<Hud::Digits>(cameraRoot, white, 5, -0.55f, 1.5f, 0.9f);
    runner->coinDigits = std::make_unique<Hud::Digits>(cameraRoot, yellow, 3, 0.72f, 1.5f, 0.9f);
    runner->coinIcon = Hud::MakeCube(cameraRoot, yellow, true, Vector3{0.16f, 0.16f, 0.04f});
    const Vector3 green{0.3f, 1.0f, 0.4f};
    const Vector3 lightBlue{0.7f, 0.8f, 1.0f};

    runner->bigScore = std::make_unique<Hud::Digits>(cameraRoot, white, 5, 0.35f, 0.0f, 1.0f);
    runner->bigBest = std::make_unique<Hud::Digits>(cameraRoot, yellow, 5, 0.35f, -0.4f, 1.0f);

    runner->title = std::make_unique<Hud::Text>(cameraRoot, yellow, 90);
    runner->record = std::make_unique<Hud::Text>(cameraRoot, green, 70);
    runner->prompt = std::make_unique<Hud::Text>(cameraRoot, white, 100);
    runner->labelScore = std::make_unique<Hud::Text>(cameraRoot, lightBlue, 50);
    runner->labelBest = std::make_unique<Hud::Text>(cameraRoot, lightBlue, 50);
    runner->light = lightTransform;
    runner->lightMarker = Hud::MakeCube(*root, Vector3{1.0f, 1.0f, 0.6f}, true, Vector3{0.3f, 0.3f, 0.3f});

    runner->topicText = std::make_unique<Hud::Text>(cameraRoot, lightBlue, Hud::Text::PixelCount(TopicTitle));
    runner->teamText = std::make_unique<Hud::Text>(cameraRoot, lightBlue, Hud::Text::PixelCount(TeamName));
    runner->lightStatus = std::make_unique<Hud::Text>(cameraRoot, white, Hud::Text::PixelCount("LIGHT OFF"));
    runner->camStatus = std::make_unique<Hud::Text>(cameraRoot, white,
        std::max(Hud::Text::PixelCount("CAM GAME"), Hud::Text::PixelCount("CAM FLY")));
    runner->helpHint = std::make_unique<Hud::Text>(cameraRoot, white, Hud::Text::PixelCount("H HELP"));

    runner->helpPanel = Hud::MakeCube(cameraRoot, Vector3{0.03f, 0.04f, 0.1f}, true, Vector3{2.4f, 4.0f, 0.01f});
    for (const HelpLine& line : HelpLines)
        runner->helpTexts.push_back(std::make_unique<Hud::Text>(cameraRoot, white, Hud::Text::PixelCount(line.text)));

    runner->magnetBar = Hud::MakeCube(cameraRoot, Vector3{0.3f, 0.9f, 1.0f}, true, Vector3{0.8f, 0.06f, 0.02f});
    runner->shieldBar = Hud::MakeCube(cameraRoot, Vector3{0.3f, 1.0f, 0.4f}, true, Vector3{0.8f, 0.06f, 0.02f});

    for (int i = 0; i < RunnerGame::MaxLives; ++i)
        runner->hearts.push_back(Hud::MakeCube(cameraRoot, Vector3{1.0f, 0.25f, 0.3f}, true, Vector3{0.14f, 0.14f, 0.03f}));

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

int main()
{
    try
    {
        return RunGame();
    }
    catch (const std::exception& e)
    {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "FATAL ERROR: unknown exception" << std::endl;
    }

    return 1;
}