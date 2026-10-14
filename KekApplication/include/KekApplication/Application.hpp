//
// Created by Dmitriy on 12.10.2026.
//

#pragma once

#include "KekPlatform/Window.hpp"
#include "KekWorld/Scene.hpp"

#include <memory>

namespace Kek
{
    class Application final
    {
        unsigned int maximumFramesPerSecond = 60;
        unsigned int tickRate = 20;

        Platform::Window window;
        std::unique_ptr<World::Scene> scene;

    public:
        explicit Application(const Platform::WindowSettings &windowSettings,
                            std::unique_ptr<World::Scene> scene)
            : window(windowSettings), scene(std::move(scene)) {}

        [[nodiscard]] unsigned int GetMaximumFramesPerSecond() const { return maximumFramesPerSecond; }
        void SetMaximumFramesPerSecond(const unsigned int maximumFramesPerSecond) { this->maximumFramesPerSecond = maximumFramesPerSecond; }

        [[nodiscard]] unsigned int GetTickRate() const { return tickRate; }
        void SetTickRate(const unsigned int tickRate) { this->tickRate = tickRate; }

        std::unique_ptr<World::Scene>& GetScene() { return scene; }
        void SetScene(std::unique_ptr<World::Scene> &scene) { this->scene = std::move(scene); }

        void Run() const;
    };
}
