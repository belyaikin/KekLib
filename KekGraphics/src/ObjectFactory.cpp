//
// Created by Dmitriy on 01.10.2026.
//

#include "KekGraphics/OpenGL/ObjectFactory.hpp"

#include "KekGraphics/OpenGL/Context.hpp"
#include "KekGraphics/OpenGL/Shader.hpp"

namespace Kek::Graphics::OpenGL
{
    std::vector<VertexArray> ObjectFactory::CreateVertexArrays(const size_t n)
    {
        std::vector<GLuint> vertexArraysIds(n);
        glGenVertexArrays(static_cast<GLsizei>(n), vertexArraysIds.data());

        std::vector<VertexArray> vertexArrays;
        vertexArrays.reserve(n);

        for (const GLuint vertexArrayId : vertexArraysIds)
        {
            vertexArrays.emplace_back(vertexArrayId);
        }

        return vertexArrays;
    }

    std::vector<Buffer> ObjectFactory::CreateBuffers(const size_t n)
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

    Shader ObjectFactory::CreateShader(const Shader::Type type, const char *source)
    {
        GLuint shaderId = 0;

        switch (type)
        {
            case Shader::Type::Vertex: shaderId = glCreateShader(GL_VERTEX_SHADER); break;
            case Shader::Type::Fragment: shaderId = glCreateShader(GL_FRAGMENT_SHADER); break;
        }

        glShaderSource(shaderId, 1, &source, nullptr);

        return Shader{shaderId, source};
    }
}