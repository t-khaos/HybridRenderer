#pragma once

#include "Just/Common.h"
#include "Just/Core/HitRecord.h"
#include "Just/Math/Vector.h"
#include "Just/Geometry/Bounds.h"
#include "Just/Core/Emitter.h"
#include "Just/Core/SampleMeshResult.h"
#include "Just/Math/Dpdf.h"
#include "Just/Core/Material.h"
#include "Just/Math/Transform.h"
#include "Just/Core/MeshVertex.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stbi/stb_image.h>
#include <stbi/stb_image_write.h>

struct Mesh {
    std::vector<Point3f> positions;
    std::vector<Point2f> texcoords;
    std::vector<Vector3f> normals;
    std::vector<size_t> indices;
    Ref<Emitter> emitter;
    Ref<Material> material;
    DiscretePDF dpdf;
    float area{};
    Bounds3f bounds;
public:
    Mesh(const std::string &path, const Transform &transform)
    {
        std::ifstream fileStream(path);
        std::string line;
        std::unordered_map<MeshVertex, size_t, MeshVertexHash> meshVertexMap;
        std::vector<Point3f> vs;
        std::vector<Point2f> vts;
        std::vector<Vector3f> vns;
        std::vector<MeshVertex> meshVertices;
        while (std::getline(fileStream, line))
        {
            std::istringstream strStream(line);
            std::string prefix;
            strStream >> prefix;
            //解析数据
            if (prefix == "v")
            {
                Point4f v4f;
                strStream >> v4f.x >> v4f.y >> v4f.z;
                v4f.w = 1;
                auto v3f = transform(v4f);
                vs.push_back(v3f);
                bounds.Expand(v3f);
            }
            else if (prefix == "vn")
            {
                Vector3f vn;
                strStream >> vn.x >> vn.y >> vn.z;
                vns.push_back(Normalize(transform(vn)));
            }
            else if (prefix == "vt")
            {
                Point2f vt;
                strStream >> vt.x >> vt.y;
                vts.push_back(vt);
            }
            else if (prefix == "f")
            {
                std::string v1, v2, v3, v4;
                strStream >> v1 >> v2 >> v3 >> v4;
                MeshVertex meshTriangle[6];
                int vNums = 3;
                meshTriangle[0] = StringToMeshVertex(v1);
                meshTriangle[1] = StringToMeshVertex(v2);
                meshTriangle[2] = StringToMeshVertex(v3);
                if (!v4.empty())
                {
                    vNums = 6;
                    meshTriangle[3] = StringToMeshVertex(v1);
                    meshTriangle[4] = StringToMeshVertex(v3);
                    meshTriangle[5] = StringToMeshVertex(v4);
                }
                for (int i = 0; i < vNums; i++)
                {
                    const MeshVertex &v = meshTriangle[i];
                    auto it = meshVertexMap.find(v);
                    if (it == meshVertexMap.end())
                    {
                        meshVertexMap[v] = meshVertices.size();
                        indices.push_back(meshVertices.size());
                        meshVertices.push_back(v);
                    }
                    else
                    {
                        indices.push_back(it->second);
                    }
                }
            }
        }
        //读取顶点坐标
        positions.resize(meshVertices.size());
        for (size_t i = 0; i < meshVertices.size(); ++i)
        {
            positions[i] = vs.at(meshVertices[i].p - 1);
        }
        //读取法线
        if (!vns.empty())
        {
            normals.resize(meshVertices.size());
            for (size_t i = 0; i < meshVertices.size(); ++i)
            {
                normals[i] = vns.at(meshVertices[i].n - 1);
            }
        }
        //读取纹理坐标
        if (!vts.empty())
        {
            texcoords.resize(meshVertices.size());
            for (size_t i = 0; i < meshVertices.size(); ++i)
            {
                texcoords[i] = vts.at(meshVertices[i].uv - 1);
            }
        }
    }

    ~Mesh() = default;

    void Active();

    std::tuple<size_t, size_t, size_t> GetTriangleIndices(size_t faceIndex) const;

    Bounds3f GetFaceBounds(size_t faceIndex) const;

    size_t GetTriangleCount() const { return indices.size() / 3; }

    float GetFaceArea(size_t faceIndex) const;

    bool RayIntersect(size_t faceIndex, const Ray &ray, HitRecord &record) const;

    SampleMeshResult SampleSurfaceUniform(const Ref<Sampler> &sampler) const;
};

void Mesh::Active() {
    if (material == nullptr) {
        material = CreateRef<Diffuse>(Color3f(0.5f,0.5f,0.5f));
    }
    area = 0.0f;
    dpdf.Reserve(GetTriangleCount());
    for (uint32_t i = 0; i < GetTriangleCount(); ++i) {
        auto faceArea = GetFaceArea(i);
        area += faceArea;
        dpdf.Append(area);
    }
    dpdf.Normalize();
}

//获取指定三角面的三个顶点的索引
std::tuple<size_t, size_t, size_t> Mesh::GetTriangleIndices(size_t faceIndex) const{
    return {indices.at( faceIndex * 3 + 0),indices.at( faceIndex * 3 + 1), indices.at( faceIndex * 3 + 2)};
}

//获取指定三角面的面积
float Mesh::GetFaceArea(size_t faceIndex) const {
    auto [idx0, idx1, idx2] = GetTriangleIndices(faceIndex);
    const Point3f p0 = positions.at(idx0);
    const Point3f p1 = positions.at(idx1);
    const Point3f p2 = positions.at(idx2);
    return 0.5f * Length(Cross((p1 - p0), (p2 - p0)));
}

//获取指定三角面的包围盒
Bounds3f Mesh::GetFaceBounds(size_t faceIndex) const {
    Bounds3f faceBounds;
    auto [A, B, C] = GetTriangleIndices(faceIndex);
    faceBounds.Expand(positions[A]);
    faceBounds.Expand(positions[B]);
    faceBounds.Expand(positions[C]);
    return faceBounds;
}

//射线相交测试
bool Mesh::RayIntersect(size_t faceIndex, const Ray &ray, HitRecord &record) const {
    auto [idx0, idx1, idx2] = GetTriangleIndices(faceIndex);
    const Point3f p0 = positions.at(idx0);
    const Point3f p1 = positions.at(idx1);
    const Point3f p2 = positions.at(idx2);

    const Vector3f edge1 = p1 - p0;
    const Vector3f edge2 = p2 - p0;

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

//表面均匀采样一个点
SampleMeshResult Mesh::SampleSurfaceUniform(const Ref<Sampler> &sampler) const {
    SampleMeshResult result;

    //随机采样一个三角形
    uint32_t idx = dpdf.sample(sampler->Next1D());
    Point3f p0 = positions.at(indices.at(idx + 0));
    Point3f p1 = positions.at(indices.at(idx + 1));
    Point3f p2 = positions.at(indices.at(idx + 2));

    //随机在三角形范围均匀采样点重心坐标
    Point2f zeta = sampler->Next2D();
    float alpha = 1 - sqrt(1 - zeta.x);
    float beta = zeta.y * sqrt(1 - zeta.x);

    result.p = p0 * alpha + p1 * beta + p2 * (1 - alpha - beta);

    if (!normals.empty()) {
        Point3f n0 = normals.at(indices.at(idx + 0));
        Point3f n1 = normals.at(indices.at(idx + 1));
        Point3f n2 = normals.at(indices.at(idx + 2));
        result.n = Normalize(alpha * n0 + beta * n1 + (1 - alpha - beta) * n2);
    } else {
        Vector3f e1 = p1 - p0;
        Vector3f e2 = p2 - p0;
        result.n = Normalize(Cross(e1, e2));
    }

    result.pdf = 1.0f / area;

    return result;
}


