//
// Created by Dmitriy on 06.10.2026.
//

#pragma once

#include <GLFW/glfw3.h>

namespace Kek::Platform
{
    class Window
    {
        using GLProcLoader = GLFWglproc (*)(const char*);
        [[nodiscard]] GLProcLoader GetProcLoader() const;
    };
}
