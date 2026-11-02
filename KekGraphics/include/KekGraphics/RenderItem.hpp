//
// Created by Dmitriy on 21.09.2026.
//

#pragma once

#include "Material.hpp"
#include "MeshBuffers.hpp"
#include "KekWorld/Transform.hpp"

namespace Kek::Graphics
{
    struct RenderItem
    {
        const MeshBuffers* mesh;
        const World::Transform* transform;
        const Material* material;
    };
}
