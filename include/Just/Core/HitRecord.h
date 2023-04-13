#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Math/Frame.h"

struct Mesh;
struct HitRecord
{
    Point3f hitPoint;
    Point2f uv;
    float hitTime;
    Frame shFrame;
    Frame geoFrame;
    Ref<Mesh> hitMesh;
    HitRecord() : hitTime(0) {}
};