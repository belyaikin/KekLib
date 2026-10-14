//
// Created by Dmitriy on 06.10.2026.
//

#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace Kek::Platform
{
    struct WindowSettings final
    {
        unsigned int width;
        unsigned int height;

        std::string title;
    };

    class Window final
    {
        using GLProcLoader = GLFWglproc (*)(const char*);
        [[nodiscard]] GLProcLoader GetProcLoader() const;

        WindowSettings settings;

        GLFWwindow *glfwWindow;

    public:
        explicit Window(const WindowSettings settings) :
            settings(settings),
            glfwWindow(
                glfwCreateWindow(
                    settings.width,
                    settings.height,
                    settings.title.c_str(),
                    nullptr,
                    nullptr
                )
            )
        {}

        void MakeContextCurrent() const;

        [[nodiscard]] bool ShouldClose() const;

        void PollEvents() const;
    };
}
