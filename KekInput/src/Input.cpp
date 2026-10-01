//
// Created by vladk on 01.10.2026.
//

#include "KekInput/Input.hpp"

#include <array>

namespace Kek::Input
{
    namespace
    {
        struct State
        {
            std::array<bool, KeyCount> held{};
            std::array<bool, KeyCount> pressed{};
            std::array<bool, KeyCount> released{};
        };

        State state;

        constexpr std::size_t Index(const Key key)
        {
            return static_cast<std::size_t>(key);
        }

        constexpr bool IsValid(const Key key)
        {
            return key != Key::Unknown && Index(key) < KeyCount;
        }
    }

    bool GetKey(const Key key)
    {
        return IsValid(key) && state.held[Index(key)];
    }

    bool GetKeyPressed(const Key key)
    {
        return IsValid(key) && state.pressed[Index(key)];
    }

    bool GetKeyReleased(const Key key)
    {
        return IsValid(key) && state.released[Index(key)];
    }

    void NewFrame()
    {
        state.pressed.fill(false);
        state.released.fill(false);
    }

    void OnKeyEvent(const Key key, const bool isDown)
    {
        if (!IsValid(key)) return;

        const auto i = Index(key);

        if (isDown && !state.held[i])
        {
            state.pressed[i] = true;
            state.held[i] = true;
        }
        else if (!isDown && state.held[i])
        {
            state.released[i] = true;
            state.held[i] = false;
        }
    }
}