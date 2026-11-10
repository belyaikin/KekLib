//
// Created by vladk on 08.11.2026.
//

#pragma once

#include <algorithm>
#include <array>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekMath/Vector3.hpp"
#include "KekWorld/Node.hpp"
#include "KekWorld/Transform.hpp"

namespace Hud
{
    using Kek::Math::Vector3;
    using Kek::World::Node;
    using Kek::World::Transform;

    constexpr float Hidden = -1000.0f;
    constexpr float Depth = -3.0f;

    inline Transform* MakeCube(Node& parent, const Vector3& color, const bool unlit,
                               const Vector3& scale = Vector3{1, 1, 1})
    {
        using namespace Kek::Graphics;

        auto node = std::make_unique<Node>();
        auto* transform = node->AttachComponent<Transform>(Vector3{0, Hidden, 0});
        transform->SetScale(scale);

        if (unlit)
            node->AttachComponent<Components::MeshDisplay>(Mesh::Cube(color), MaterialSettings{.fragmentShader = "shaders/unlit.frag"});
        else
            node->AttachComponent<Components::MeshDisplay>(Mesh::Cube(color));

        parent.AddChild(std::move(node));
        return transform;
    }

    class Digits final
    {
        struct Segment
        {
            Transform* transform = nullptr;
            Vector3 position{0, 0, 0};
        };

        std::vector<std::array<Segment, 7>> digits;
        int maxValue = 9;
        int shown = -1;
        bool visible = false;

        static void Put(const Segment& segment, const bool on)
        {
            segment.transform->SetPosition(on ? segment.position : Vector3{segment.position.X(), Hidden, segment.position.Z()});
        }

    public:
        Digits(Node& parent, const Vector3& color, const int count, const float centerX, const float centerY, const float scale)
        {
            const float w = 0.12f * scale, h = 0.22f * scale, t = 0.03f * scale;
            const float spacing = 0.17f * scale;

            struct Def { float x, y, sx, sy; };
            const Def defs[7] = {
                {0.0f, h / 2,w, t},      // a (top)
                {w / 2,h / 4,t,h / 2},  // b (top right)
                {w / 2,-h / 4,t,h / 2},  // c (bottom right)
                {0.0f,-h / 2,w,t},      // d (bottom)
                {-w / 2,-h / 4,t,h / 2},  // e (bottom left)
                {-w / 2,h / 4,t,h / 2},  // f (top left)
                {0.0f,0.0f,w,t},      // g (middle)
            };

            for (int i = 0; i < count; ++i)
            {
                const float dx = centerX + (static_cast<float>(i) - static_cast<float>(count - 1) * 0.5f) * spacing;

                std::array<Segment, 7> digit{};
                for (int s = 0; s < 7; ++s)
                {
                    digit[s].transform = MakeCube(parent, color, true, Vector3{defs[s].sx, defs[s].sy, 0.01f});
                    digit[s].position = Vector3{dx + defs[s].x, centerY + defs[s].y, Depth};
                }
                digits.push_back(digit);
            }

            maxValue = 1;
            for (int i = 0; i < count; ++i) maxValue *= 10;
            maxValue -= 1;
        }

        void Show(int value)
        {
            value = std::clamp(value, 0, maxValue);
            if (visible && value == shown) return;
            visible = true;
            shown = value;

            // bit 0 = a, bit 1 = b, ... bit 6 = g
            static constexpr unsigned Masks[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

            int divisor = (maxValue + 1) / 10;
            bool started = false;

            for (std::size_t i = 0; i < digits.size(); ++i)
            {
                const int d = (value / divisor) % 10;
                divisor = std::max(1, divisor / 10);

                started = started || d != 0 || i + 1 == digits.size(); // no leading zeros
                const unsigned mask = started ? Masks[d] : 0u;

                for (int s = 0; s < 7; ++s)
                    Put(digits[i][s], ((mask >> s) & 1u) != 0);
            }
        }

        void Hide()
        {
            if (!visible) return;
            visible = false;
            shown = -1;
            for (auto& digit : digits)
                for (auto& segment : digit)
                    Put(segment, false);
        }
    };

    // Pixel text (letters A B C D E G M N O P R S T U V W and space).
    class Text final
    {
        using Rows = std::array<const char*, 5>;

        std::vector<Transform*> pixels;
        std::string shownText;
        float shownX = 0, shownY = 0, shownSize = 0;
        bool visible = false;

        static const Rows* Glyph(const char c)
        {
            static const std::map<char, Rows> glyphs = {
                {'A', Rows{".X.", "X.X", "XXX", "X.X", "X.X"}},
                {'B', Rows{"XX.", "X.X", "XX.", "X.X", "XX."}},
                {'C', Rows{".XX", "X..", "X..", "X..", ".XX"}},
                {'D', Rows{"XX.", "X.X", "X.X", "X.X", "XX."}},
                {'E', Rows{"XXX", "X..", "XX.", "X..", "XXX"}},
                {'G', Rows{".XX", "X..", "X.X", "X.X", ".XX"}},
                {'M', Rows{"X.X", "XXX", "XXX", "X.X", "X.X"}},
                {'N', Rows{"XX.", "X.X", "X.X", "X.X", "X.X"}},
                {'O', Rows{"XXX", "X.X", "X.X", "X.X", "XXX"}},
                {'P', Rows{"XX.", "X.X", "XX.", "X..", "X.."}},
                {'R', Rows{"XX.", "X.X", "XX.", "X.X", "X.X"}},
                {'S', Rows{".XX", "X..", ".X.", "..X", "XX."}},
                {'T', Rows{"XXX", ".X.", ".X.", ".X.", ".X."}},
                {'U', Rows{"X.X", "X.X", "X.X", "X.X", "XXX"}},
                {'V', Rows{"X.X", "X.X", "X.X", "X.X", ".X."}},
                {'W', Rows{"X.X", "X.X", "XXX", "XXX", "X.X"}},
            };

            const auto it = glyphs.find(c);
            return it == glyphs.end() ? nullptr : &it->second;
        }

    public:
        Text(Node& parent, const Vector3& color, const int capacity)
        {
            for (int i = 0; i < capacity; ++i)
                pixels.push_back(MakeCube(parent, color, true));
        }

        void Show(const std::string& text, const float cx, const float cy, const float pixel)
        {
            if (visible && text == shownText && cx == shownX && cy == shownY && pixel == shownSize) return;
            visible = true;
            shownText = text;
            shownX = cx;
            shownY = cy;
            shownSize = pixel;

            const float columns = static_cast<float>(text.size()) * 4.0f - 1.0f;
            std::size_t used = 0;

            for (std::size_t ci = 0; ci < text.size(); ++ci)
            {
                const Rows* glyph = Glyph(text[ci]);
                if (glyph == nullptr) continue;

                for (int row = 0; row < 5; ++row)
                {
                    for (int col = 0; col < 3; ++col)
                    {
                        if ((*glyph)[row][col] != 'X' || used >= pixels.size()) continue;

                        const float x = cx + (static_cast<float>(ci * 4 + col) - (columns - 1.0f) * 0.5f) * pixel;
                        const float y = cy + static_cast<float>(2 - row) * pixel;

                        pixels[used]->SetScale(Vector3{pixel * 0.92f, pixel * 0.92f, 0.02f});
                        pixels[used]->SetPosition(Vector3{x, y, Depth});
                        ++used;
                    }
                }
            }

            for (; used < pixels.size(); ++used)
                pixels[used]->SetPosition(Vector3{0, Hidden, Depth});
        }

        void Hide()
        {
            if (!visible) return;
            visible = false;
            for (Transform* pixel : pixels)
                pixel->SetPosition(Vector3{0, Hidden, Depth});
        }
    };
}