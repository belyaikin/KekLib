//
// Created by Dmitriy on 12.10.2026.
//

#include "KekApplication/Application.hpp"

#include <chrono>
#include <stdexcept>
#include <thread>

namespace Kek
{
    // TODO: Create ApplicationLoop class or something like that to not put all of this here
    void Application::Run()
    {
        using Clock = std::chrono::steady_clock;
        using Seconds = std::chrono::duration<double>;

        this->window.MakeContextCurrent();
        if (!this->glContext.Initialize(this->window.GetProcLoader()))
        throw std::runtime_error("Failed to load OpenGL functions");

        this->renderer = std::make_unique<Graphics::Renderer>();


        this->root->Start();

        const Seconds tickInterval{1.0 / tickRate};
        constexpr Seconds maxFrameTime{0.25};

        auto previous = Clock::now();
        Seconds accumulator{0};

        while (!this->window.ShouldClose())
        {
            const auto frameStart = Clock::now();
            Seconds frameTime = frameStart - previous;

            previous = frameStart;

            if (frameTime > maxFrameTime) frameTime = maxFrameTime;

            accumulator += frameTime;

            this->window.PollEvents();

            while (accumulator >= tickInterval)
            {
                root->Tick();
                accumulator -= tickInterval;
            }

            const double alpha = accumulator / tickInterval;
            (void)alpha;

            // TODO: render here
            
            const auto framebufferSize = this->window.GetFramebufferSize();
            this->renderer->Resize(framebufferSize.width, framebufferSize.height);

            this->renderer->Render(*this->root);

            this->window.SwapBuffers();

            if (maximumFramesPerSecond > 0)
            {
                const auto frameDuration = std::chrono::duration_cast<Clock::duration>(
                    Seconds{1.0 / maximumFramesPerSecond});
                std::this_thread::sleep_until(frameStart + frameDuration);
            }
        }
    }
}
