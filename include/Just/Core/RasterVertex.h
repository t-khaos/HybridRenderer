#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"

struct RasterVertex
{
    Point3f pos; //3d
    Point4f pos4; //4d pos
    Point2f pos2f; //2d pos float
    Point2i pos2i; //2d pos int32
    float rhw{}; // 1/w
    Point2f texcoord;
    Vector3f normal;
};
inline std::tuple<float, float, float> CalcBarycentric(RasterVertex *triangle, float x, float y)
{
    auto &A = triangle[0].pos2f;
    auto &B = triangle[1].pos2f;
    auto &C = triangle[2].pos2f;
    auto P = Point2f{x, y};
    float numerator, denominator;
    numerator = (A.y - C.y) * P.x + (C.x - A.x) * P.y + A.x * C.y - C.x * A.y;
    denominator = (A.y - C.y) * B.x + (C.x - A.x) * B.y + A.x * C.y - C.x * A.y;
    float beta = numerator / denominator;
    numerator = (A.y - B.y) * P.x + (B.x - A.x) * P.y + A.x * B.y - B.x * A.y;
    denominator = (A.y - B.y) * C.x + (B.x - A.x) * C.y + A.x * B.y - B.x * A.y;
    float gamma = numerator / denominator;
    return {1.0f - beta - gamma, beta, gamma};
}