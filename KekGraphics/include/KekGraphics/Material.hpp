//
// Created by vladk on 23.10.2026.
//

#pragma once
#include "KekMath/Vector3.hpp"

namespace Kek::Graphics
{
    struct Material final
    {
        Math::Vector3 color{1, 1, 1};
        float ambient = 0.1f;
        float diffuse = 1.0f;
        float specular = 0.5f;
        float shininess = 32.0f;
    };
}