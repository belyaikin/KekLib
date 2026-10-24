//
// Created by readmao on 24.10.2026.
//

#include "KekGraphics/Renderer.hpp"

#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekGraphics/Mesh.hpp"
#include "KekGraphics/OpenGL/Context.hpp"
#include "KekGraphics/OpenGL/ObjectFactory.hpp"

namespace Kek::Graphics
{
    using OpenGL::Context;
    using OpenGL::ObjectFactory;
    using OpenGL::Shader;

    namespace
    {
        constexpr const char* VertexSource = R"(#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec3 vColor;

void main()
{
    vColor = aColor;
    gl_Position = vec4(aPosition, 1.0);
}
)";

        constexpr const char* FragmentSource = R"(#version 330 core
in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
)";

        OpenGL::ShaderProgram CreateDefaultProgram()
        {
            const Shader vertex = ObjectFactory::CreateShader(Shader::Type::Vertex, VertexSource);
            const Shader fragment = ObjectFactory::CreateShader(Shader::Type::Fragment, FragmentSource);

            return ObjectFactory::CreateShaderProgram({&vertex, &fragment});
        }
    }

    Renderer::Renderer() : program(CreateDefaultProgram())
    {
        Context::SetClearColor(0.10f, 0.20f, 0.40f, 1.0f);
    }

    void Renderer::Render(const World::Node& root)
    {
        Context::Clear();
        Context::UseProgram(program);

        DrawNode(root);
    }

    void Renderer::DrawNode(const World::Node& node)
    {
        if (const auto* display = node.GetComponent<Components::MeshDisplay>())
        {
            if (const Mesh* mesh = display->GetMesh())
                mesh->Draw();
        }

        for (const auto& child : node.GetChildren())
            DrawNode(*child);
    }
}
