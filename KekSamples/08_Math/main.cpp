//
// Created by readmao on 24.10.2026.
//
// Console-only checks (no window). Each commit that touches KekMath/Transform adds checks here.
#include <cmath>
#include <iostream>

#include "KekMath/Vector3.hpp"

using Kek::Math::Vector3;

namespace
{
    int failures = 0;

    void Check(const bool condition, const char* name)
    {
        std::cout << (condition ? "[ ok ] " : "[FAIL] ") << name << '\n';
        if (!condition) ++failures;
    }

    bool Near(const float a, const float b) { return std::fabs(a - b) < 1e-4f; }

    bool Near(const Vector3& a, const Vector3& b)
    {
        return Near(a.X(), b.X()) && Near(a.Y(), b.Y()) && Near(a.Z(), b.Z());
    }
}

int main()
{
    //Vector3
    Check(Near(Vector3{3, 2, 1} - Vector3{1, 1, 1}, Vector3{2, 1, 0}), "Vector3 subtraction");
    Check(Near(-Vector3{1, -2, 3}, Vector3{-1, 2, -3}), "Vector3 unary minus");
    Check(Near(Vector3{1, 2, 3}.Dot(Vector3{4, 5, 6}), 32.0f), "Vector3 dot product");
    Check(Near(Vector3::right.Cross(Vector3::up), Vector3{0, 0, 1}), "Vector3 cross: right x up = +Z");
    Check(Near(Vector3{3, 4, 0}.Length(), 5.0f), "Vector3 length");
    Check(Near(Vector3{0, 0, 5}.Normalized(), Vector3{0, 0, 1}), "Vector3 normalized");
    Check(Near(Vector3{0, 0, 0}.Normalized(), Vector3{0, 0, 0}), "Vector3 normalized zero vector stays zero");

    std::cout << (failures == 0 ? "ALL OK\n" : "SOME CHECKS FAILED\n");
    return failures == 0 ? 0 : 1;
}