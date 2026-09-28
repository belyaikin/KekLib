//
// Created by Dmitriy on 28.09.2026.
//

#pragma once

#include "Node.hpp"
#include "KekRenderer/IRenderTarget.hpp"

namespace Kek::Nodes
{
    class RenderableNode : public Node, public Renderer::IRenderTarget
    {

    };
}
