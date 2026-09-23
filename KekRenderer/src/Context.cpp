//
// Created by Dmitriy on 22.09.2026.
//

#include "KekRenderer/OpenGL/Context.hpp"

namespace Kek::Renderer::OpenGL
{
    std::vector<VertexArray> Context::CreateVertexArrays(const int n)
    {
        std::vector<GLuint> vertexArraysIds(n);
        glGenVertexArrays(n, vertexArraysIds.data());

        std::vector<VertexArray> vertexArrays;
        vertexArrays.reserve(n);

        for (const GLuint vertexArrayId : vertexArraysIds)
        {
            vertexArrays.emplace_back(vertexArrayId);
        }

        return vertexArrays;
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

    std::vector<Buffer> Context::CreateBuffers(const size_t n)
    {
        std::vector<GLuint> buffersIds(n);
        glGenBuffers(static_cast<GLsizei>(n), buffersIds.data());

        std::vector<Buffer> buffers;
        buffers.reserve(n);

        for (const GLuint bufferId: buffersIds)
        {
            buffers.emplace_back(bufferId);
        }

        return buffers;
    }
}
