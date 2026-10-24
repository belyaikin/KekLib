//
// Created by Dmitriy on 12.10.2026.
//

#pragma once
#include "KekGraphics/OpenGL/Context.hpp"
#include "KekPlatform/Window.hpp"
#include "KekWorld/Node.hpp"

#include "KekGraphics/Renderer.hpp"

#include <memory>
#include <utility>

namespace Kek
{
    class Application final
    {
        unsigned int maximumFramesPerSecond = 60;
        unsigned int tickRate = 20;

        Platform::Window window;
        Graphics::OpenGL::Context glContext;

        // Created in Run(), once OpenGL is loaded. Declared before `root`, so it is destroyed after the scene
        // and before the window.
        std::unique_ptr<Graphics::Renderer> renderer;
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

        void Run();
    };
}
