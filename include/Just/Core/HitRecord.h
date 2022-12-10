#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Math/Frame.h"

struct HitRecord
{
    Point3f point;
    Point2f uv;
    float hitTime;
    Frame shadingFrame;
    Frame geometryFrame;
    size_t meshIndex;
    HitRecord() : hitTime(0), meshIndex(-1) {}
};