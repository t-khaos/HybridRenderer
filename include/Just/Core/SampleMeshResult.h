#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Math/Frame.h"

struct SampleMeshResult {
    Point3f p;
    Vector3f n;
    float pdf;
};