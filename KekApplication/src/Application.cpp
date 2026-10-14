//
// Created by Dmitriy on 12.10.2026.
//

#include "KekApplication/Application.hpp"

#include <thread>

namespace Kek
{
    // TODO: Create ApplicationLoop class or something like that to not put all of this here
    void Application::Run() const
    {
        using Clock = std::chrono::steady_clock;
        using Seconds = std::chrono::duration<double>;

        this->window.MakeContextCurrent();

        const Seconds tickInterval{1.0 / tickRate};
        const Seconds maxFrameTime{0.25};

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
                scene->Tick();
                accumulator -= tickInterval;
            }

            const double alpha = accumulator / tickInterval;
            (void)alpha;

            // TODO: render here

            if (maximumFramesPerSecond > 0)
            {
                const auto frameDuration = std::chrono::duration_cast<Clock::duration>(
                    Seconds{1.0 / maximumFramesPerSecond});
                std::this_thread::sleep_until(frameStart + frameDuration);
            }
        }
    }
}
