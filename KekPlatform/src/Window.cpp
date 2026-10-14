//
// Created by Dmitriy on 12.10.2026.
//

#include "KekPlatform/Window.hpp"

namespace Kek::Platform
{
    Window::GLProcLoader Window::GetProcLoader() const
    {
        return &glfwGetProcAddress;
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
}
