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
}
