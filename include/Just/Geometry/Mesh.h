#pragma once

#include "Just/Common.h"
#include "Just/Core/HitRecord.h"
#include "Just/Math/Vector.h"
#include "Just/Geometry/Bounds.h"

struct Mesh
{
    std::vector<Point3f> positions;
    std::vector<Point2f> texcoords;
    std::vector<Vector3f> normals;
    std::vector<size_t> indices;
public:
    Mesh() = default;
    ~Mesh() = default;
    Bounds3f bounds;
    Bounds3f GetFaceBounds(size_t faceIndex) const;
    std::tuple<size_t, size_t, size_t> GetTriangle(size_t faceIndex) const
    {
        size_t index = faceIndex * 3;
        return {indices[index], indices[index + 1], indices[index + 2]};
    }
    bool RayIntersect(size_t faceIndex, const Ray &ray, HitRecord &record) const;
};

Bounds3f Mesh::GetFaceBounds(size_t faceIndex) const
{
    Bounds3f faceBounds;
    auto [A, B, C] = GetTriangle(faceIndex);
    faceBounds.Expand(positions[A]);
    faceBounds.Expand(positions[B]);
    faceBounds.Expand(positions[C]);
    return faceBounds;
}
bool Mesh::RayIntersect(size_t faceIndex, const Ray &ray, HitRecord &record) const
{
    //读取三角形顶点坐标
    auto [A, B, C] = GetTriangle(faceIndex);
    const Point3f &p0 = positions[A];
    const Point3f &p1 = positions[B];
    const Point3f &p2 = positions[C];
    const Vector3f edge1 = p1 - p0;
    const Vector3f edge2 = p2 - p0;

    //计算交点
    const Vector3f pvec = Cross(ray.direction, edge2);
    float det = Dot(edge1, pvec);
    if (det < 1e-8f && det > -1e-8f)return false;
    float invDet = 1.0f / det;
    const Vector3f tvec = ray.origin - p0;
    const float u = Dot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f)return false;
    const Vector3f qvec = Cross(tvec, edge1);
    const float v = Dot(ray.direction, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f)return false;
    const float t = Dot(edge2, qvec) * invDet;
    if (t < 0 || t > ray.tMax)return false;
    ray.tMax = t;
    record.hitTime = t;
    record.uv = Point2f(u, v);
    return true;
}



