//
// Created by readmao on 24.10.2026.
//

#include "KekGraphics/Components/MeshDisplay.hpp"

namespace Kek::Graphics::Components
{
    void MeshDisplay::Start()
    {
        mesh = std::make_unique<GpuMesh>(model);
    }
}