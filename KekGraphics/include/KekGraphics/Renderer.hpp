
#pragma once

#include "KekWorld/Node.hpp"
#include "OpenGL/Objects/ShaderProgram.hpp"
#include "KekMath/Matrix4.hpp"
#include "Components/Camera.hpp"

namespace Kek::Graphics
{
    /// @brief Draws a node tree. Must be created while the OpenGL context is current.
    class Renderer final
    {
        OpenGL::Objects::ShaderProgram program;

        int viewportWidth = 1;
        int viewportHeight = 1;

        [[nodiscard]] static const Components::Camera* FindCamera(const World::Node& node);

        void DrawNode(const World::Node& node, const Math::Matrix4& parentWorld);

    public:
        Renderer();

        /// @brief Tells the renderer the size (in pixels) of the framebuffer it draws into.
        void Resize(int width, int height);

        /// @brief Clears the screen and draws every MeshDisplay found in the tree.
        void Render(const World::Node& root);
    };
}