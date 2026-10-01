#pragma once

#include <cstddef>

namespace Kek::Input
{
    enum class Key : unsigned short
    {
        Unknown = 0,

        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        Up, Down, Left, Right,

        Space, Enter, Escape, Tab, Backspace,
        LeftShift, RightShift,
        LeftCtrl, RightCtrl,
        LeftAlt, RightAlt,

        Count
    };

    inline constexpr std::size_t KeyCount = static_cast<std::size_t>(Key::Count);
}