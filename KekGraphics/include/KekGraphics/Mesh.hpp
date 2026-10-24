#pragma once

#include "Model.hpp"
#include "OpenGL/Buffer.hpp"
#include "OpenGL/VertexArray.hpp"

namespace Kek::Graphics
{
    /// @brief Geometry uploaded to the GPU. Must be created while the OpenGL context is current.
    class Mesh final
    {
        OpenGL::VertexArray vertexArray;
        OpenGL::Buffer vertexBuffer;
        OpenGL::Buffer indexBuffer;

        GLsizei count = 0;
        bool indexed = false;

    public:
        explicit Mesh(const Model& model);

        [[nodiscard]] GLsizei GetCount() const { return count; }

        void Draw() const;
    };
}