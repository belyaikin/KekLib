
#pragma once

#include "KekWorld/Node.hpp"
#include "OpenGL/ShaderProgram.hpp"

namespace Kek::Graphics
{
    /// @brief Draws a node tree. Must be created while the OpenGL context is current.
    class Renderer final
    {
        OpenGL::ShaderProgram program;

        void DrawNode(const World::Node& node);

    public:
        Renderer();

        /// @brief Clears the screen and draws every MeshDisplay found in the tree.
        void Render(const World::Node& root);
    };
}