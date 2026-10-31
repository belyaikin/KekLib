//
// Created by Dmitriy on 29.09.2026.
//

#pragma once

#include <type_traits>

#include "KekMath/Vector2.hpp"
#include "KekMath/Vector3.hpp"

namespace Kek::Graphics
{
    struct Vertex final
    {
        Math::Vector3 position;
        Math::Vector3 color;
        Math::Vector3 normal;
        Math::Vector2 uv;


        constexpr explicit Vertex(const Math::Vector3& position,
                                  const Math::Vector3& normal,
                                  const Math::Vector3& color = Math::Vector3{1, 1, 1,},
                                  const Math::Vector2& uv = Math::Vector2{0, 0})
            : position(position), color(color), normal(normal), uv(uv) {}
    };

    static_assert(sizeof(Vertex) == 11 * sizeof(float), "Vertex must be tightly packed");
    static_assert(std::is_trivially_copyable_v<Vertex>, "Vertex is memcpy'd into a GPU buffer");
}