#pragma once

#include "Just/Common.h"
#include "Just/Core/Emitter.h"
#include "Just/Core/SampleMeshResult.h"
#include "Just/Geometry/Mesh.h"

struct AreaLight : Emitter {

private:
    Color3f radiance;
public:
    AreaLight(const Color3f &radiance) : radiance(radiance) {}

    Color3f Sample(const Ref<Mesh> &mesh, EmitterQueryRecord &eRec, const Ref<Sampler> &sampler) const override;

    float Pdf(const Ref<Mesh> &mesh, const EmitterQueryRecord &eRec) const override;

    Color3f Eval(const EmitterQueryRecord &eRec) const override;
};

Color3f AreaLight::Sample(const Ref<Mesh> &mesh, EmitterQueryRecord &eRec, const Ref<Sampler> &sampler) const {
    SampleMeshResult point = mesh->SampleSurfaceUniform(sampler);
    eRec.hitPoint = point.p;
    eRec.normal = point.n;
    eRec.wi = Normalize(eRec.samplePoint - eRec.hitPoint);
    eRec.pdf = Pdf(mesh, eRec);

    if (eRec.pdf > 0.0f && !std::isnan(eRec.pdf) && !std::isinf(eRec.pdf)) {
        return Eval(eRec) / eRec.pdf;
    }

    return Color3f(0.f);
}

float AreaLight::Pdf(const Ref<Mesh> &mesh, const EmitterQueryRecord &eRec) const {
    if (Dot(eRec.normal, -eRec.wi) > 0.f) {
        return mesh->dpdf.GetNormalization();
    }
    return 0.f;
}

Color3f AreaLight::Eval(const EmitterQueryRecord &eRec) const {
    return (Dot(eRec.normal, -eRec.wi) > 0.f) ? radiance : Color3f(0, 0, 0);//背面采样点剔除
}