#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Geometry/Ray.h"

template<typename T>
struct Bounds2
{
    Point2<T> pMin, pMax;
    Bounds2() : pMin(std::numeric_limits<T>::max()), pMax(std::numeric_limits<T>::lowest()) {}
    Bounds2(const Point2<T> &pMin, const Point2<T> &pMax) : pMin(pMin), pMax(pMax) {}
    explicit Bounds2(const Point2<T> &p) : pMin(p), pMax(p) {}
    Bounds2(T xMin, T yMin, T xMax, T yMax) : pMin(xMin, yMin), pMax(xMax, yMax) {}
    void Expand(const Point2<T> &point)
    {
        pMin = Min(pMin, point);
        pMax = Max(pMax, point);
    }
    void Expand(const Bounds2<T> &bounds)
    {
        pMin = Min(pMin, bounds.pMin);
        pMax = Max(pMax, bounds.pMax);
    }
    void Clamp(const Bounds2<T> &bounds)
    {
        pMin.x = std::clamp(pMin.x, bounds.pMin.x, bounds.pMax.x);
        pMin.y = std::clamp(pMin.y, bounds.pMin.y, bounds.pMax.y);
        pMax.x = std::clamp(pMax.x, bounds.pMin.x, bounds.pMax.x);
        pMax.y = std::clamp(pMax.y, bounds.pMin.y, bounds.pMax.y);
    }
};

template<typename T>
struct Bounds3
{
    Point3<T> pMin, pMax;
    Bounds3() : pMin(std::numeric_limits<float>::max()),
                pMax(std::numeric_limits<float>::lowest()) {}
    explicit Bounds3(const Point3f &p) : pMin(p), pMax(p) {}
    Bounds3(const Point3<T> &p1, const Point3<T> &p2)
            : pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
              pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)) {}
    const Point3<T>& operator[](size_t i) const {
        return (&pMin)[i];
    }

    Point3f& operator[](size_t i) {
        return (&pMin)[i];
    }
    //扩展包围盒
    void Expand(const Point3<T> &p)
    {
        pMin = Min(pMin, p);
        pMax = Max(pMax, p);
    }
    //扩展包围盒
    void Expand(const Bounds3<T> &b)
    {
        pMin = Min(pMin, b.pMin);
        pMax = Max(pMax, b.pMax);
    }
    //射线相交测试
    bool RayIntersect(const Ray &ray) const
    {
        Vector3f invDir = 1.0f / ray.direction;
        Vector3f t0 = (pMin - ray.origin) * invDir;
        Vector3f t1 = (pMax - ray.origin) * invDir;
        Vector3f tMin = Min(t0, t1);
        Vector3f tMax = Max(t0, t1);
        float tNear = std::max(MaxComponent(tMin), ray.tMin);
        float tFar = std::min(MinComponent(tMax), ray.tMax);
        return tNear <= tFar;
    }
    //包围盒拐角点
    Point3<T> Corner(int i) const {
        return {
                (*this)[(i & 1)].origin,
                (*this)[(i & 2)].target,
                (*this)[(i & 4)].z
        };
    }
    //包围盒中心坐标点
    Point3<T> Centroid() const { return (pMax + pMin) * 0.5f; }
    //包围盒对角线向量
    Vector3<T> Diagonal() const { return pMax - pMin; }
    //包围盒最长维度
    size_t MajorAxis() const
    {
        Vector3<T> d = Diagonal();
        return (d.x > d.y) ? ((d.x > d.z) ? 0 : 2) : ((d.y > d.z) ? 1 : 2);
    }
    //包围盒表面积
    float SurfaceArea() const
    {
        Vector3<T> d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }
    //包围盒体积
    float Volume() const
    {
        Vector3<T> d = Diagonal();
        return d.x * d.y * d.z;
    }
    bool Inside(const Vector3<T> &p) const
    {
        return p.x >= pMin.x && p.x <= pMax.x &&
               p.y >= pMin.y && p.y <= pMax.y &&
               p.z >= pMin.z && p.z <= pMax.z;
    }
    bool InsideExclusive(const Vector3<T> &p) const
    {
        return p.x >= pMin.x && p.x < pMax.x &&
               p.y >= pMin.y && p.y < pMax.y &&
               p.z >= pMin.z && p.z < pMax.z;
    }
};

using Bounds2f = Bounds2<float>;
using Bounds2i = Bounds2<int>;
using Bounds3f = Bounds3<float>;
//合并包围盒
template<typename T>
inline Bounds3<T> Union(const Bounds3<T> &b1, const Bounds3<T> &b2)
{
    return {
            Min(b1.pMin, b2.pMin),
            Max(b1.pMax, b2.pMax)
    };
}
//包围盒相交部分
template<typename T>
inline Bounds3<T> Intersect(const Bounds3<T> &b1, const Bounds3<T> &b2)
{
    return {
            Max(b1.pMin, b2.pMin),
            Min(b1.pMax, b2.pMax)
    };
}
//包围盒覆盖测试
template<typename T>
inline bool Overlaps(const Bounds3<T> &b1, const Bounds3<T> &b2)
{
    bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
    return x && y && z;
}
