//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include <vector>
#include <glad/gl.h>

namespace Kek::Renderer {
    class Renderer {
        std::vector<GLuint> vertexBufferObjects;
        std::vector<GLuint> vertexArrayObjects;
    public:
        void AddVertexBufferObject(GLuint vertexBufferObject);
        void AddVertexArrayObject(GLuint vertexArrayObject);
    };
} // KekRenderer