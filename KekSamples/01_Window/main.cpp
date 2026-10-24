//
// Created by readmao on 24.10.2026.
//
#include <memory>

#include "KekApplication/Application.hpp"

int main()
{
    Kek::Application app(
        Kek::Platform::WindowSettings{800, 600, "KekLib - 01 Window"},
        std::make_unique<Kek::World::Node>()
    );

    app.Run();
    return 0;
}
