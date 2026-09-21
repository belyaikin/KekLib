//
// Created by Dmitriy on 21.09.2026.
//

#pragma once
#include "Scene.h"

namespace Kek::World {
    class Node {
        const Scene &scene;

    public:
        virtual void Start();
        virtual void Update();
    };
}
