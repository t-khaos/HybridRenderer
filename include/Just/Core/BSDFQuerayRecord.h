#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"

struct BSDFQueryRecord {
    Vector3f wi;
    Vector3f wo;
    float eta;

    BSDFQueryRecord(const Vector3f &wi)
            : wi(wi), eta(1.f) {}

    BSDFQueryRecord(const Vector3f &wi, const Vector3f &wo)
            : wi(wi), wo(wo), eta(1.f) {}
};