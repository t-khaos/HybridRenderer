#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"

struct Frame
{
    Vector3f s, t, n;
    Frame() = default;
    explicit Frame(const Vector3f &n) : n(n) { CoordinateSystem(n, &s, &t); }
    ~Frame() = default;
    Frame(const Vector3f &s, const Vector3f &t, const Vector3f &n) : s(s), t(t), n(n) {}
    Vector3f ToLocal(const Vector3f &v) const { return {Dot(v, s), Dot(v, t), Dot(v, n)}; }
    Vector3f ToWorld(const Vector3f &v) const { return s * v.x + t * v.y + n * v.z; }
    static float AbsCosTheta(const Vector3f &v) { return std::abs(v.z); }
    static float CosTheta(const Vector3f &v) { return v.z; }
    static float Cos2Theta(const Vector3f &v) { return v.z * v.z; }
    //注意避免因浮点数误差导致的极小可能的负值
    static float Sin2Theta(const Vector3f &v) { return std::max(0.0f, 1.0f - Cos2Theta(v)); }
    static float SinTheta(const Vector3f &v) { return std::sqrt(Sin2Theta(v)); }
    static float TanTheta(const Vector3f &v) { return SinTheta(v) / CosTheta(v); }
    static float Tan2Theta(const Vector3f &v) { return Sin2Theta(v) / Cos2Theta(v); }
    static float CosPhi(const Vector3f &v)
    {
        float sinTheta = SinTheta(v);
        return (sinTheta == 0) ? 1 : std::clamp(v.x / sinTheta, -1.0f, 1.0f);
    }
    static float SinPhi(const Vector3f &v)
    {
        float sinTheta = SinTheta(v);
        return (sinTheta == 0) ? 0 : std::clamp(v.y / sinTheta, -1.0f, 1.0f);
    }
    static float Cos2Phi(const Vector3f &v) { return CosPhi(v) * CosPhi(v); }
    static float Sin2Phi(const Vector3f &v) { return SinPhi(v) * SinPhi(v); }
};

