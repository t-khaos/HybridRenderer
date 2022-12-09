#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Math/Matrix.h"

struct Transform
{
    Matrix4f matrix;
    Matrix4f inverse;
    Transform()
            : matrix(Matrix4f::Identity()), inverse(Matrix4f::Identity()) {}
    explicit Transform(const Matrix4f &matrix)
            : matrix(matrix), inverse(Inverse(matrix)) {}
    Transform(std::initializer_list<std::initializer_list<float>> list) : matrix(list), inverse(Inverse(matrix)) {}
    Transform(const Matrix4f &matrix, const Matrix4f &inverse)
            : matrix(matrix), inverse(inverse) {}
    ~Transform() = default;
    Point3f operator()(const Point4f &point) const
    {
        auto homogenized = matrix * point;
        return Point3f{homogenized / homogenized.w};
    }
    Vector3f operator()(const Vector3f &vector) const
    {
        return matrix * vector;
    }
    Transform operator*(const Transform &rhs) const
    {
        return {matrix * rhs.matrix, rhs.inverse * inverse};
    }
};

//变换的逆
inline Transform Inverse(const Transform &t)
{
    return {t.inverse, t.matrix};
}
//变换的转置
inline Transform Transpose(const Transform &t)
{
    return {Transpose(t.matrix), Transpose(t.inverse)};
}

//旋转变换

inline Transform RotateX(float theta)
{
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    return {
            {1, 0,        0,         0},
            {0, cosTheta, -sinTheta, 0},
            {0, sinTheta, cosTheta,  0},
            {0, 0,        0,         1}
    };
}
inline Transform RotateY(float theta)
{
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    return {
            {cosTheta,  0, sinTheta, 0},
            {0,         1, 0,        0},
            {-sinTheta, 0, cosTheta, 0},
            {0,         0, 0,        1}
    };
}
inline Transform RotateZ(float theta)
{
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    return {
            {cosTheta, -sinTheta, 0, 0},
            {sinTheta, cosTheta,  0, 0},
            {0,        0,         1, 0},
            {0,        0,         0, 1}
    };
}
inline Transform Rotate(float x, float y, float z)
{
    return RotateZ(z) * RotateY(y) * RotateX(x);
}
inline Transform Scale(float x, float y, float z)
{
    return {
            {x, 0, 0, 0},
            {0, y, 0, 0},
            {0, 0, z, 0},
            {0, 0, 0, 1}
    };
}
inline Transform Translate(const Vector3f &delta)
{
    return {
            {1, 0, 0, delta.x},
            {0, 1, 0, delta.y},
            {0, 0, 1, delta.z},
            {0, 0, 0, 1}
    };
}
inline Transform LookAt(const Point3f &origin, const Point3f &target, const Vector3f &worldUp)
{
    Vector3f dir = Normalize(target - origin);
    Vector3f right = Normalize(Cross(Normalize(worldUp), dir));
    Vector3f up = Cross(dir, right);
    return {
            {right.x, up.x, dir.x, origin.x},
            {right.y, up.y, dir.y, origin.y},
            {right.z, up.z, dir.z, origin.z},
            {0,       0,    0,     1}
    };
}
//正交投影变换
inline Transform Orthographic(float n, float f)
{
    return Scale(1, 1, 1.0f / (f - n)) * Translate({0, 0, -n});
}
//透视投影变换
inline Transform Perspective(float fov, float n, float f)
{
    //右手系
    float recip = 1.0f / (f - n);
    float cot = 1.0f / std::tan(Radians(fov) / 2.0f);
    return {
            {cot, 0,   0,         0},
            {0,   cot, 0,         0},
            {0,   0,   f * recip, -f * n * recip},
            {0,   0,   1,         0}
    };
}
inline Transform ScreenMapping(const Point2i &resolution)
{
    auto w = static_cast<float>(resolution.x);
    auto h = static_cast<float>(resolution.y);
    return {
            {w / 2, 0,     0, w / 2},
            {0,     h / 2, 0, h / 2},
            {0,     0,     1, 0},
            {0,     0,     0, 1}
    };
}
