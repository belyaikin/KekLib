//
// Created by readmao on 24.10.2026.
//

#include "KekGraphics/Renderer.hpp"

#include "KekGraphics/Material.hpp"
#include "KekGraphics/RenderRegistry.hpp"
#include "KekGraphics/OpenGL/Context.hpp"
#include "KekWorld/Transform.hpp"

namespace Kek::Graphics
{
    using OpenGL::Context;
    using OpenGL::Objects::ShaderProgram;

    namespace
    {
        /// Everything that is the same for all objects in one frame.
        struct FrameData
        {
            Math::Matrix4 view = Math::Matrix4::Identity();
            Math::Matrix4 projection = Math::Matrix4::Identity();
            Math::Vector3 viewPosition{0, 0, 0};
            const Components::Light* light = nullptr;
        };
        // Uniforms a program does not declare are ignored by OpenGL(location -1),
        // so an unlit shader can simply skip the light uniforms.
        void ApplyFrameUniforms(const ShaderProgram& program, const FrameData& frame)
        {
            program.SetMatrix4("uView", frame.view);
            program.SetMatrix4("uProjection", frame.projection);
            program.SetVector3("uViewPos", frame.viewPosition);
            program.SetBool("uLightingEnabled", frame.light != nullptr);
            if (frame.light != nullptr)
            {
                program.SetVector3("uLightPos", frame.light->GetPosition());
                program.SetVector3("uLightColor", frame.light->GetColor() * frame.light->GetIntensity());
            }
        }
    }

    Renderer::Renderer()
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
        FrameData frame;

        if (const Components::Camera* camera = RenderRegistry::GetCamera())
        {
            const float aspectRatio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight > 0 ? viewportHeight : 1);

            frame.view = camera->GetViewMatrix();
            frame.projection = camera->GetProjectionMatrix(aspectRatio);
            frame.viewPosition = camera->GetPosition();
        }

        frame.light = RenderRegistry::GetLight();
        const ShaderProgram* current = nullptr;

        for (const RenderRegistry::Entry& entry : RenderRegistry::GetItems())
        {
            const RenderItem& item = entry.item;
            if (!item.mesh || !item.material)
                continue;

            const ShaderProgram& program = item.material->GetProgram();
            // Switch programs only when the object needs a different one.
            if (&program != current)
            {
                Context::UseProgram(program);
                ApplyFrameUniforms(program, frame);
                current = &program;
            }

            const Math::Matrix4 world = item.transform ? item.transform->GetWorldMatrix() : Math::Matrix4::Identity();

            program.SetMatrix4("uModel", world);
            item.material->Apply();
            Context::BindVertexArray(item.mesh->GetVertexArray());

            if (item.mesh->IsIndexed())
                Context::DrawElements(item.mesh->GetCount());
            else
                Context::DrawArrays(item.mesh->GetCount());

            Context::UnbindVertexArray();
        }
    }
}

