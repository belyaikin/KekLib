//
// Created by Dmitriy on 12.10.2026.
//

#pragma once

#include "KekPlatform/Window.hpp"
#include "KekWorld/Node.hpp"

#include <memory>

namespace Kek
{
    class Application final
    {
        unsigned int maximumFramesPerSecond = 60;
        unsigned int tickRate = 20;

        Platform::Window window;
        std::unique_ptr<World::Node> root;

    public:
        explicit Application(const Platform::WindowSettings &windowSettings,
                            std::unique_ptr<World::Node> scene)
            : window(windowSettings), root(std::move(scene)) {}

        [[nodiscard]] unsigned int GetMaximumFramesPerSecond() const { return maximumFramesPerSecond; }
        void SetMaximumFramesPerSecond(const unsigned int maximumFramesPerSecond) { this->maximumFramesPerSecond = maximumFramesPerSecond; }

        [[nodiscard]] unsigned int GetTickRate() const { return tickRate; }
        void SetTickRate(const unsigned int tickRate) { this->tickRate = tickRate; }

        std::unique_ptr<World::Node>& GetRootNode() { return root; }
        void SetRootNode(std::unique_ptr<World::Node> &root) { this->root = std::move(root); }

        void Run() const;
    };
}
