#pragma once

#include "Just/Common.h"

struct MeshVertex
{
    size_t p = size_t(-1);
    size_t uv = size_t(-1);
    size_t n = size_t(-1);
    MeshVertex() = default;
    bool operator==(const MeshVertex &rhs) const
    {
        return p == rhs.p && uv == rhs.uv && n == rhs.n;
    }
};

struct MeshVertexHash
{
    size_t operator()(const MeshVertex &v) const
    {
        size_t hash = std::hash<size_t>()(v.p);
        hash = hash * 37 + std::hash<size_t>()(v.uv);
        hash = hash * 37 + std::hash<size_t>()(v.n);
        return hash;
    }
};

inline MeshVertex StringToMeshVertex(const std::string &str)
{
    MeshVertex vert;
    auto list = Tokenize(str, "/", true);
    vert.p = StringToNumeric<size_t>(list[0]);
    if (list.size() >= 2 && !list[1].empty())
    {
        vert.uv = StringToNumeric<size_t>(list[1]);
    }
    if (list.size() >= 3 && !list[2].empty())
    {
        vert.n = StringToNumeric<size_t>(list[2]);
    }
    return vert;
}