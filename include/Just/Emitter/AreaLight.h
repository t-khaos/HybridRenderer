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
    eRec.p = point.p;
    eRec.n = point.n;
    eRec.wi = Normalize(eRec.ref_p - eRec.p);
    eRec.pdf = Pdf(mesh, eRec);

    if (eRec.pdf > 0.0f && !std::isnan(eRec.pdf) && !std::isinf(eRec.pdf)) {
        return Eval(eRec) / eRec.pdf;
    }

    return Color3f(0.f);
}

float AreaLight::Pdf(const Ref<Mesh> &mesh, const EmitterQueryRecord &eRec) const {
    float costTheta = Dot(eRec.n,-eRec.wi);
    if (costTheta <= 0.0f)
    {
        return 0.0f;
    }
    //光源pdf转为立体角上
    return mesh->dpdf.GetNormalization() * LengthSquare(eRec.p - eRec.ref_p) / costTheta;
}

Color3f AreaLight::Eval(const EmitterQueryRecord &eRec) const {
    return (Dot(eRec.n,eRec.wi) < 0.0f) ? radiance : Color3f(0.0f);//法线和wi要面朝不同方向，这样光源正面才朝着着色点
}