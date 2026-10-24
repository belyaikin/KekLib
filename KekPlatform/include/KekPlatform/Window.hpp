//
// Created by Dmitriy on 06.10.2026.
//

#pragma once

#include <string>

// Forward declaration: keeps <GLFW/glfw3.h> out of public headers,
// so only KekPlatform itself needs GLFW's include directories.
struct GLFWwindow;

namespace Kek::Platform
{
    struct WindowSettings final
    {
        unsigned int width;
        unsigned int height;

        std::string title;
    };

    struct FramebufferSize final
    {
        int width;
        int height;
    };

    class Window final
    {
        WindowSettings settings;

        GLFWwindow *glfwWindow = nullptr;

    public:
        using GLProcLoader = void* (*)(const char*);

        /// @brief Initializes GLFW (if needed) and creates a window with an OpenGL 3.3 core context.
        /// @throws std::runtime_error if GLFW or the window cannot be created.
        explicit Window(const WindowSettings& settings);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void MakeContextCurrent() const;

        /// @brief Function that resolves OpenGL entry points (pass it to the GL loader).
        [[nodiscard]] GLProcLoader GetProcLoader() const;

        [[nodiscard]] bool ShouldClose() const;

        void PollEvents() const;

        void SwapBuffers() const;

        [[nodiscard]] FramebufferSize GetFramebufferSize() const;
    };
}