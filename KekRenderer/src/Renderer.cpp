//
// Created by Dmitriy on 21.09.2026.
//

#include "KekRenderer/Renderer.h"

namespace Kek::Renderer {
    void Renderer::AddVertexBufferObject(const GLuint vertexBufferObject) {
        this->vertexBufferObjects.push_back(vertexBufferObject);
    }

    void Renderer::AddVertexArrayObject(GLuint vertexArrayObject) {
        this->vertexArrayObjects.push_back(vertexArrayObject);
    }
} // KekRenderer