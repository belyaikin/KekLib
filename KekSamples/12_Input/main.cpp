//
// Created by readmao on 25.10.2026.
//
#include <iostream>
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekInput/Input.hpp"

using Kek::Input::Key;

// Reports keyboard state from Tick(). Click the window and press W / Space.
struct InputReporter final : Kek::World::Component
{
    int heldTicks = 0;

    void Tick() override
    {
        if (Kek::Input::GetKeyPressed(Key::W)) std::cout << "W pressed\n";
        if (Kek::Input::GetKeyReleased(Key::W)) std::cout << "W released (was held for " << heldTicks << " ticks)\n";
        if (Kek::Input::GetKeyPressed(Key::Space)) std::cout << "Space pressed\n";
        if (Kek::Input::GetKeyReleased(Key::Space)) std::cout << "Space released\n";

        heldTicks = Kek::Input::GetKey(Key::W) ? heldTicks + 1 : 0;
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();
    root->AttachComponent<InputReporter>();

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 12 Input"}, std::move(root));
    app.Run();
    return 0;
}