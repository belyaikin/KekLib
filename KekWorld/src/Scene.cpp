//
// Created by Dmitriy on 14.10.2026.
//

#include "KekWorld/Scene.hpp"

namespace Kek::World
{
    void Scene::Tick()
    {
        masterNode->Tick();

        for (Node* n : pendingRemoval)
            n->GetParentNode()->RemoveChildNode(n);

        pendingRemoval.clear();
    }
}
