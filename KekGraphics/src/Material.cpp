//
// Created by vladk on 01.11.2026.
//

#include "KekGraphics/Material.hpp"
#include "KekAssets/Assets.hpp"
#include "KekGraphics/OpenGL/Context.hpp"
#include "KekGraphics/OpenGL/ObjectFactory.hpp"
#include "KekGraphics/ShaderLibrary.hpp"

namespace Kek::Graphics
{
    Material::Material(const MaterialSettings& settings) : program(ShaderLibrary::Load(settings.vertexShader, settings.fragmentShader))
    {
        if (!settings.texture.empty())
        {
            const Assets::Image image = Assets::ReadImage(settings.texture);
            texture = std::make_unique<OpenGL::Objects::Texture>(OpenGL::ObjectFactory::CreateTexture(image.width, image.height, image.pixels.data()));
        }
    }
    void Material::Apply() const
    {
        program->SetBool("uHasTexture", texture != nullptr);
        if (texture != nullptr)
        {
            OpenGL::Context::BindTexture(*texture, 0);
            program->SetInt("uTexture", 0);
        }
    }
}
