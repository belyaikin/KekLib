#pragma once

#include "Key.hpp"

namespace Kek::Input
{
    [[nodiscard]] bool GetKey(Key key);

    [[nodiscard]] bool GetKeyPressed(Key key);

    [[nodiscard]] bool GetKeyReleased(Key key);

    void NewFrame();

    void OnKeyEvent(Key key, bool isDown);
}