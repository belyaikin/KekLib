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

    void Context::UnbindVertexArray()
    {
        glBindVertexArray(0);
    }

    namespace
    {
        GLenum ToGLTarget(const Buffer::Type type)
        {
            switch (type)
            {
                case Buffer::Type::Vertex: return GL_ARRAY_BUFFER;
                case Buffer::Type::Index: return GL_ELEMENT_ARRAY_BUFFER;
            }
            return GL_ARRAY_BUFFER;
        }
    }

    void Context::BindBuffer(const Buffer& buffer, const Buffer::Type type)
    {
        glBindBuffer(ToGLTarget(type), buffer.GetId());
    }

    void Context::BufferData(const Buffer::Type type, const void* data, const std::size_t bytes)
    {
        glBufferData(ToGLTarget(type), static_cast<GLsizeiptr>(bytes), data, GL_STATIC_DRAW);
    }

    void Context::EnableVertexAttribute(const GLuint index, const GLint components,
                                        const std::size_t stride, const std::size_t offset)
    {
        glVertexAttribPointer(index, components, GL_FLOAT, GL_FALSE,
                              static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
        glEnableVertexAttribArray(index);
    }

    void Context::DrawArrays(const GLsizei vertexCount)
    {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    void Context::DrawElements(const GLsizei indexCount)
    {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

    GLenum Context::GetError()
    {
        return glGetError();
    }

    void Context::UseProgram(const ShaderProgram& program)
    {
        glUseProgram(program.GetId());
    }

    void Context::SetViewport(const int x, const int y, const int width, const int height)
    {
        glViewport(x, y, width, height);
    }

    void Context::SetClearColor(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }

    void Context::EnableDepthTest()
 {
        glEnable(GL_DEPTH_TEST);
 }

    void Context::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}