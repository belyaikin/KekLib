#pragma once

#include "Mesh.hpp"
#include "OpenGL/Objects/Buffer.hpp"
#include "OpenGL/Objects/VertexArray.hpp"

namespace Kek::Graphics
{
    /// @brief Geometry uploaded to the GPU. Must be created while the OpenGL context is current.
    class MeshBuffers final
    {
        OpenGL::Objects::VertexArray vertexArray;
        OpenGL::Objects::Buffer vertexBuffer;
        OpenGL::Objects::Buffer indexBuffer;

        GLsizei count = 0;
        bool indexed = false;

    public:
        explicit MeshBuffers(const Mesh& model);

        [[nodiscard]] const OpenGL::Objects::VertexArray& GetVertexArray() const { return vertexArray; }
        [[nodiscard]] GLsizei GetCount() const { return count; }
        [[nodiscard]] bool IsIndexed() const { return indexed; }
    };
}
