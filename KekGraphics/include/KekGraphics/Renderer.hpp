
#pragma once

#include "OpenGL/Objects/ShaderProgram.hpp"
#include "KekMath/Matrix4.hpp"

namespace Kek::Graphics
{
    /// @brief Draws registered render items. Must be created while the OpenGL context is current.
    class Renderer final
    {
        OpenGL::Objects::ShaderProgram program;

        int viewportWidth = 1;
        int viewportHeight = 1;

    public:
        Renderer();

        /// @brief Tells the renderer the size (in pixels) of the framebuffer it draws into.
        void Resize(int width, int height);

        /// @brief Clears the screen and draws the registered render items.
        void Render();
    };
}
