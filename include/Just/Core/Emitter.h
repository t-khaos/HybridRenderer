#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Math/Color.h"
#include "Just/Core/Sampler.h"

struct EmitterQueryRecord {
    Vector3f hitPoint{};
    Point3f samplePoint{};
    Vector3f normal{};
    Vector3f wi{};
    float pdf{};

    explicit EmitterQueryRecord(const Point3f &x) : hitPoint(x) {}

    EmitterQueryRecord(const Point3f &ori, const Point3f &tar, const Vector3f &n) : hitPoint(ori), samplePoint(tar), normal(n) {
        wi = Normalize(tar - ori);
    }
};

struct Mesh;

struct Emitter {
    virtual Color3f Sample(const Ref<Mesh>& mesh, EmitterQueryRecord &eRec, const Ref<Sampler>& sampler) const = 0;

    virtual float Pdf(const  Ref<Mesh>& mesh, const EmitterQueryRecord &eRec) const = 0;

    virtual Color3f Eval(const EmitterQueryRecord &eRec) const = 0;
};

