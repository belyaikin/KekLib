//
// Created by readmao on 24.10.2026.
//

#include "KekGraphics/Renderer.hpp"

#include <string>

#include "KekAssets/Assets.hpp"
#include "KekGraphics/RenderRegistry.hpp"
#include "KekGraphics/OpenGL/Context.hpp"
#include "KekGraphics/OpenGL/ObjectFactory.hpp"
#include "KekWorld/Transform.hpp"

namespace Kek::Graphics
{
    using OpenGL::Context;
    using OpenGL::ObjectFactory;
    using OpenGL::Objects::Shader;
    using OpenGL::Objects::ShaderProgram;

    namespace
    {
        ShaderProgram CreateDefaultProgram()
        {
            const std::string vertexSource = Assets::ReadText("shaders/phong.vert");
            const std::string fragmentSource = Assets::ReadText("shaders/phong.frag");

            const Shader vertex = ObjectFactory::CreateShader(Shader::Type::Vertex, vertexSource.c_str());
            const Shader fragment = ObjectFactory::CreateShader(Shader::Type::Fragment, fragmentSource.c_str());

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

    void Renderer::Render()
    {
        Context::Clear();
        Context::UseProgram(program);

        // Without a camera everything is drawn directly in clip space (identity view and projection).
        Math::Matrix4 view = Math::Matrix4::Identity();
        Math::Matrix4 projection = Math::Matrix4::Identity();
        Math::Vector3 viewPosition{0, 0, 0};

        if (const Components::Camera* camera = RenderRegistry::GetCamera())
        {
            const float aspectRatio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight > 0 ? viewportHeight : 1);

            view = camera->GetViewMatrix();
            projection = camera->GetProjectionMatrix(aspectRatio);
            viewPosition = camera->GetPosition();
        }

        program.SetMatrix4("uView", view);
        program.SetMatrix4("uProjection", projection);
        program.SetVector3("uViewPos", viewPosition);

        const Components::Light* light = RenderRegistry::GetLight();
        program.SetBool("uLightingEnabled", light != nullptr);

        if (light != nullptr)
        {
            program.SetVector3("uLightPos", light->GetPosition());
            program.SetVector3("uLightColor", light->GetColor() * light->GetIntensity());
        }

        for (const RenderRegistry::Entry& entry : RenderRegistry::GetItems())
        {
            const RenderItem& item = entry.item;
            const auto& [mesh, transform] = item;

            if (!mesh)
                continue;

            const Math::Matrix4 world = transform ? transform->GetWorldMatrix() : Math::Matrix4::Identity();
            program.SetMatrix4("uModel", world);
            Context::BindVertexArray(mesh->GetVertexArray());

            if (mesh->IsIndexed())
                Context::DrawElements(mesh->GetCount());
            else
                Context::DrawArrays(mesh->GetCount());

            Context::UnbindVertexArray();
        }
    }
}