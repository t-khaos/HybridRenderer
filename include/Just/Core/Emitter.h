#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Math/Color.h"
#include "Just/Core/Sampler.h"

struct EmitterQueryRecord {
    Vector3f origin{};
    Point3f target{};
    Vector3f normal{};
    Vector3f wi{};
    float pdf{};

    explicit EmitterQueryRecord(const Point3f &x) : origin(x) {}

    EmitterQueryRecord(const Point3f &x, const Point3f &y, const Vector3f &n) : origin(x), target(y), normal(n) {
        wi = Normalize(y - x);
    }
};

struct Mesh;

struct Emitter {
    virtual Color3f sample(const Mesh *mesh, EmitterQueryRecord &eRec, Sampler *&sample) const = 0;

    virtual float pdf(const Mesh *mesh, const EmitterQueryRecord &eRec) const = 0;

    virtual Color3f eval(const EmitterQueryRecord &eRec) const = 0;
};