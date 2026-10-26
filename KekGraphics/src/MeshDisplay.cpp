//
// Created by readmao on 24.10.2026.
//

#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekGraphics/RenderRegistry.hpp"
#include "KekWorld/Node.hpp"
#include "KekWorld/Transform.hpp"

namespace Kek::Graphics::Components
{
    void MeshDisplay::Start()
    {
        buffers = std::make_unique<MeshBuffers>(model);
        RenderRegistry::Register(this, RenderItem{buffers.get(), GetNode()->GetComponent<World::Transform>()});
    }

    void MeshDisplay::Destroy()
    {
        RenderRegistry::Unregister(this);
        buffers.reset();
    }
}
