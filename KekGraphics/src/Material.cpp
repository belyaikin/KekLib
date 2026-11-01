//
// Created by vladk on 01.11.2026.
//

#include "KekGraphics/Material.hpp"
#include "KekGraphics/ShaderLibrary.hpp"

namespace Kek::Graphics
{
    Material::Material(const MaterialSettings& settings) : program(ShaderLibrary::Load(settings.vertexShader, settings.fragmentShader))
    {

    }
}
