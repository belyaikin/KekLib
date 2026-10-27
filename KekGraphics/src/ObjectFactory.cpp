//
// Created by Dmitriy on 01.10.2026.
//

#include <stdexcept>
#include <string>
#include <utility>

#include "KekGraphics/OpenGL/ObjectFactory.hpp"

namespace Kek::Graphics::OpenGL
{
    namespace
    {
        std::string ReadInfoLog(const GLuint id, const bool isProgram)
        {
            GLint length = 0;
            if (isProgram) glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
            else glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

            std::string log(static_cast<size_t>(length > 0 ? length : 1), '\0');
            GLsizei written = 0;

            if (isProgram) glGetProgramInfoLog(id, static_cast<GLsizei>(log.size()), &written, log.data());
            else glGetShaderInfoLog(id, static_cast<GLsizei>(log.size()), &written, log.data());

            log.resize(static_cast<size_t>(written));
            return log;
        }
    }

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
    VertexArray ObjectFactory::CreateVertexArray()
    {
        GLuint id = 0;
        glGenVertexArrays(1, &id);
        return VertexArray{id};
    }

    Buffer ObjectFactory::CreateBuffer()
    {
        GLuint id = 0;
        glGenBuffers(1, &id);
        return Buffer{id};
    }

    Shader ObjectFactory::CreateShader(const Shader::Type type, const char *source)
    {
        GLuint shaderId = 0;

        switch (type)
        {
            case Shader::Type::Vertex: shaderId = glCreateShader(GL_VERTEX_SHADER); break;
            case Shader::Type::Fragment: shaderId = glCreateShader(GL_FRAGMENT_SHADER); break;
        }

        Shader shader{shaderId};

        glShaderSource(shaderId, 1, &source, nullptr);
        glCompileShader(shaderId);

        GLint compiled = 0;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);

        if (compiled == GL_FALSE)
            throw std::runtime_error("Shader compilation failed:\n" + ReadInfoLog(shaderId, false));

        return shader;
    }

    ShaderProgram ObjectFactory::CreateShaderProgram(std::vector<const Shader *> shaders)
    {
        ShaderProgram shaderProgram{ glCreateProgram() };

        for (const auto shader : shaders)
        {
            glAttachShader(shaderProgram.GetId(), shader->GetId());
        }

        glLinkProgram(shaderProgram.GetId());

        GLint linked = 0;
        glGetProgramiv(shaderProgram.GetId(), GL_LINK_STATUS, &linked);

        if (linked == GL_FALSE)
            throw std::runtime_error("Shader program linking failed:\n" + ReadInfoLog(shaderProgram.GetId(), true));

        for (const auto shader : shaders)
        {
            glDetachShader(shaderProgram.GetId(), shader->GetId());
        }

        return shaderProgram;
    }
}