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
};

inline float AbsCosTheta(const Vector3f &v) { return std::abs(v.z); }
inline float CosTheta(const Vector3f &v) { return v.z; }
inline float Cos2Theta(const Vector3f &v) { return v.z * v.z; }
//注意避免因浮点数误差导致的极小可能的负值
inline float Sin2Theta(const Vector3f &v) { return std::max(0.0f, 1.0f - Cos2Theta(v)); }
inline float SinTheta(const Vector3f &v)
{
    return std::sqrt(Sin2Theta(v));
}
inline float TanTheta(const Vector3f &v)
{
    return SinTheta(v) / CosTheta(v);
}
inline float Tan2Theta(const Vector3f &v)
{
    return Sin2Theta(v) / Cos2Theta(v);
}
inline float CosPhi(const Vector3f &v)
{
    float sinTheta = SinTheta(v);
    return (sinTheta == 0) ? 1 : std::clamp(v.x / sinTheta, -1.0f, 1.0f);
}
inline float SinPhi(const Vector3f &v)
{
    float sinTheta = SinTheta(v);
    return (sinTheta == 0) ? 0 : std::clamp(v.y / sinTheta, -1.0f, 1.0f);
}
inline float Cos2Phi(const Vector3f &v)
{
    return CosPhi(v) * CosPhi(v);
}
inline float Sin2Phi(const Vector3f &v)
{
    return SinPhi(v) * SinPhi(v);
}