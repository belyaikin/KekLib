//
// Created by readmao on 24.10.2026.
//
#include "KekGraphics/MeshBuffers.hpp"

#include <cstddef>

#include "KekGraphics/OpenGL/Context.hpp"
#include "KekGraphics/OpenGL/ObjectFactory.hpp"

namespace Kek::Graphics
{
    using OpenGL::Objects::Buffer;
    using OpenGL::Context;
    using OpenGL::ObjectFactory;

    MeshBuffers::MeshBuffers(const Mesh& model) :
        vertexArray(ObjectFactory::CreateVertexArray()),
        vertexBuffer(ObjectFactory::CreateBuffer()),
        indexBuffer(ObjectFactory::CreateBuffer())
    {
        const auto& vertices = model.GetVertices();
        const auto& indices = model.GetIndices();

        Context::BindVertexArray(vertexArray);

        Context::BindBuffer(vertexBuffer, Buffer::Type::Vertex);
        Context::BufferData(Buffer::Type::Vertex, vertices.data(), vertices.size() * sizeof(Vertex));

        Context::EnableVertexAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
        Context::EnableVertexAttribute(1, 3, sizeof(Vertex), offsetof(Vertex, color));

        if (!indices.empty())
        {
            // The element buffer binding is stored inside the VAO, so it is bound while the VAO is bound.
            Context::BindBuffer(indexBuffer, Buffer::Type::Index);
            Context::BufferData(Buffer::Type::Index, indices.data(), indices.size() * sizeof(unsigned int));

            indexed = true;
            count = static_cast<GLsizei>(indices.size());
        }
        else
        {
            count = static_cast<GLsizei>(vertices.size());
        }

        Context::UnbindVertexArray();
    }

}
