//
// Created by readmao on 24.10.2026.
//
// Console-only checks (no window). Each commit that touches KekMath/Transform adds checks here.
#include <cmath>
#include <iostream>


#include "KekMath/Vector3.hpp"
#include "KekMath/Matrix4.hpp"

using Kek::Math::Vector3;
using Kek::Math::Matrix4;

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

    //Matrix4
    constexpr float HalfPi = 1.5707963f;

    Check(Near(Matrix4::Identity().TransformPoint(Vector3{1, 2, 3}), Vector3{1, 2, 3}), "Matrix4 identity keeps a point");
    Check(Near(Matrix4::Translation(Vector3{1, 0, 0}).TransformPoint(Vector3{1, 2, 3}), Vector3{2, 2, 3}), "Matrix4 translation");
    Check(Near(Matrix4::Scale(Vector3{2, 3, 4}).TransformPoint(Vector3{1, 1, 1}), Vector3{2, 3, 4}), "Matrix4 scale");
    Check(Near(Matrix4::RotationZ(HalfPi).TransformPoint(Vector3{1, 0, 0}), Vector3{0, 1, 0}), "Matrix4 RotationZ(90deg): +X -> +Y");
    Check(Near(Matrix4::RotationY(HalfPi).TransformPoint(Vector3{1, 0, 0}), Vector3{0, 0, -1}), "Matrix4 RotationY(90deg): +X -> -Z");
    Check(Near(Matrix4::RotationX(HalfPi).TransformPoint(Vector3{0, 1, 0}), Vector3{0, 0, 1}), "Matrix4 RotationX(90deg): +Y -> +Z");

    const auto translateThenScale = Matrix4::Translation(Vector3{1, 0, 0}) * Matrix4::Scale(Vector3{2, 2, 2});
    Check(Near(translateThenScale.TransformPoint(Vector3{1, 1, 1}), Vector3{3, 2, 2}), "Matrix4 a*b applies b first (scale, then translate)");

    Check(Near(Matrix4::Translation(Vector3{5, 0, 0}).Data()[12], 5.0f), "Matrix4 is column-major (translation x is Data()[12])");

    const auto rotation = Matrix4::Rotation(Vector3{0.3f, 0.5f, 0.7f});
    Check(Near((rotation.Transposed() * rotation).TransformPoint(Vector3{1, 2, 3}), Vector3{1, 2, 3}), "Matrix4 rotation transposed is its inverse");

    const auto projection = Matrix4::Perspective(1.0471976f, 1.0f, 0.1f, 100.0f);
    Check(Near(projection.TransformPoint(Vector3{0, 0, -0.1f}).Z(), -1.0f), "Matrix4 perspective: near plane -> z = -1");
    Check(Near(projection.TransformPoint(Vector3{0, 0, -100.0f}).Z(), 1.0f), "Matrix4 perspective: far plane -> z = +1");

    std::cout << (failures == 0 ? "ALL OK\n" : "SOME CHECKS FAILED\n");
    return failures == 0 ? 0 : 1;
}