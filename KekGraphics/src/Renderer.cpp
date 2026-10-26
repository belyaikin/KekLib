//
// Created by readmao on 24.10.2026.
//

#include "KekGraphics/Renderer.hpp"

#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekGraphics/GpuMesh.hpp"
#include "KekGraphics/OpenGL/Context.hpp"
#include "KekGraphics/OpenGL/ObjectFactory.hpp"

namespace Kek::Graphics
{
    using OpenGL::Context;
    using OpenGL::ObjectFactory;
    using OpenGL::Objects::Shader;


    namespace
    {
        /// TODO: Create KekFiles and provide the source of the shaders from files.
        constexpr const char* VertexSource = R"(#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vColor;

void main()
{
    vColor = aColor;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
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

        ShaderProgram CreateDefaultProgram()
        {
            const Shader vertex = ObjectFactory::CreateShader(Shader::Type::Vertex, VertexSource);
            const Shader fragment = ObjectFactory::CreateShader(Shader::Type::Fragment, FragmentSource);

            return ObjectFactory::CreateShaderProgram({&vertex, &fragment});
        }
    }

    Renderer::Renderer() : program(CreateDefaultProgram())
    {
        Context::SetClearColor(0.10f, 0.20f, 0.40f, 1.0f);
        Context::EnableDepthTest();
    }

    void Renderer::Resize(const int width, const int height)
    {
        if (width == viewportWidth && height == viewportHeight) return;

        viewportWidth = width;
        viewportHeight = height;

        Context::SetViewport(0, 0, width, height);
    }

    void Renderer::Render(const World::Node& root)
    {
        Context::Clear();
        Context::UseProgram(program);


        // Without a camera everything is drawn directly in clip space (identity view and projection).
        Math::Matrix4 view = Math::Matrix4::Identity();
        Math::Matrix4 projection = Math::Matrix4::Identity();

        if (const Components::Camera* camera = FindCamera(root))
        {
            const float aspectRatio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight > 0 ? viewportHeight : 1);

            view = camera->GetViewMatrix();
            projection = camera->GetProjectionMatrix(aspectRatio);
        }

        program.SetMatrix4("uView", view);
        program.SetMatrix4("uProjection", projection);

        DrawNode(root, Math::Matrix4::Identity());
    }
    const Components::Camera* Renderer::FindCamera(const World::Node& node)
    {
        if (const auto* camera = node.GetComponent<Components::Camera>())
            return camera;

        for (const auto& child : node.GetChildren())
        {
            if (const auto* camera = FindCamera(*child))
                return camera;
        }

        return nullptr;
    }

    void Renderer::DrawNode(const World::Node& node, const Math::Matrix4& parentWorld)
    {
        // A node is positioned relative to its parent, so world = parentWorld * local.
        const Math::Matrix4 world = parentWorld * node.GetTransform()->GetLocalMatrix();

        if (const auto* display = node.GetComponent<Components::MeshDisplay>())
        {
            if (const GpuMesh* mesh = display->GetMesh())
            {
                program.SetMatrix4("uModel", world);
                mesh->Draw();
            }
        }

        for (const auto& child : node.GetChildren())
            DrawNode(*child, world);
    }
}
