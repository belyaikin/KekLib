//
// Created by Dmitriy on 12.10.2026.
//

#include "KekPlatform/Window.hpp"

#include <stdexcept>

#include <GLFW/glfw3.h>

namespace Kek::Platform
{
    namespace
    {
        // GLFW is initialized by the first window and terminated with the last one.
        int liveWindows = 0;

        void* LoadProc(const char* name)
        {
            return reinterpret_cast<void*>(glfwGetProcAddress(name));
        }
    }

    Window::Window(const WindowSettings& settings) : settings(settings)
    {
        if (liveWindows == 0 && !glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        glfwWindow = glfwCreateWindow(
            static_cast<int>(settings.width),
            static_cast<int>(settings.height),
            settings.title.c_str(),
            nullptr,
            nullptr
        );

        if (glfwWindow == nullptr)
        {
            if (liveWindows == 0) glfwTerminate();
            throw std::runtime_error("Failed to create a window (OpenGL 3.3 core is required)");
        }

        ++liveWindows;
    }

    Window::~Window()
    {
        glfwDestroyWindow(glfwWindow);

        if (--liveWindows == 0) glfwTerminate();
    }

    Window::GLProcLoader Window::GetProcLoader() const
    {
        return &LoadProc;
    }

    void Window::MakeContextCurrent() const
    {
        glfwMakeContextCurrent(this->glfwWindow);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(this->glfwWindow);
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(this->glfwWindow);
    }

    FramebufferSize Window::GetFramebufferSize() const
    {
        FramebufferSize size{};
        glfwGetFramebufferSize(this->glfwWindow, &size.width, &size.height);
        return size;
    }
}