//
// Created by Dmitriy on 29.09.2026.
//

#pragma once

#include <type_traits>

#include "KekMath/Vector3.hpp"

namespace Kek::Graphics
{
    /// @brief A single mesh vertex. The memory layout is uploaded to the GPU as is:
    /// 3 floats of position followed by 3 floats of color.
    struct Vertex final
    {
        Math::Vector3 position;
        Math::Vector3 color;

        constexpr explicit Vertex(const Math::Vector3& position,
                                  const Math::Vector3& color = Math::Vector3{1, 1, 1})
            : position(position), color(color) {}
    };

    static_assert(sizeof(Vertex) == 6 * sizeof(float), "Vertex must be tightly packed");
    static_assert(std::is_trivially_copyable_v<Vertex>, "Vertex is memcpy'd into a GPU buffer");
}