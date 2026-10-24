//
// Created by Dmitriy on 22.09.2026.
//

#include "KekGraphics/OpenGL/Context.hpp"

namespace Kek::Graphics::OpenGL
{
    bool Context::Initialize(const LoaderFunc loader) const
    {
        return gladLoadGL(reinterpret_cast<GLADloadfunc>(loader)) != 0;
    }

    void Context::BindVertexArray(const VertexArray& vertexArray)
    {
        glBindVertexArray(vertexArray.GetId());
    }

    void Context::DestroyVertexArrays(const std::vector<VertexArray>& vertexArrays)
    {
        std::vector<GLuint> ids;
        ids.reserve(vertexArrays.size());

        for (const auto& vertexArray : vertexArrays) {
            ids.push_back(vertexArray.GetId());
        }

        glDeleteVertexArrays(static_cast<GLsizei>(ids.size()), ids.data());
    }

    void Context::BindBuffer(const Buffer& buffer, const Buffer::Type type) {
        switch (type) {
            case Buffer::Type::Vertex: glBindBuffer(GL_ARRAY_BUFFER, buffer.GetId());
        }
    }
    void Context::SetClearColor(const float r, const float g, const float b, const float a)
{
    glClearColor(r, g, b, a);
    }

     void Context::Clear()
    {
      glClear(GL_COLOR_BUFFER_BIT);
    }
}
