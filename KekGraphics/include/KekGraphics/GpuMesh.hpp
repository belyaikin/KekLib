#pragma once

#include "Model.hpp"
#include "OpenGL/Objects/Buffer.hpp"
#include "OpenGL/Objects/VertexArray.hpp"

namespace Kek::Graphics
{
    /// @brief Geometry uploaded to the GPU. Must be created while the OpenGL context is current.
    class GpuMesh final
    {
        OpenGL::Objects::VertexArray vertexArray;
        OpenGL::Objects::Buffer vertexBuffer;
        OpenGL::Objects::Buffer indexBuffer;

        GLsizei count = 0;
        bool indexed = false;

    public:
        explicit GpuMesh(const Model& model);

        [[nodiscard]] GLsizei GetCount() const { return count; }

        void Draw() const;
    };
}