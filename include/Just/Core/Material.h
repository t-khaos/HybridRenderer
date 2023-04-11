#pragma once

#include "Just/Common.h"
#include "Just/Core/BSDFQuerayRecord.h"
#include "Just/Math/Warp.h"

struct Material
{
public:
    virtual Color3f Sample(BSDFQueryRecord &bRec, const Point2f &sample) const = 0;
    virtual Color3f Eval(const BSDFQueryRecord &bRec) const = 0;
    virtual float Pdf(const BSDFQueryRecord &bRec) const = 0;
    virtual bool IsDiffuse() const { return false; }
};

struct Diffuse : public Material {
private:
    Color3f albedo;
public:
    Diffuse(const Color3f& albedo) : albedo(albedo) {}
    Color3f Eval(const BSDFQueryRecord &bRec) const override {
        if (Frame::CosTheta(bRec.wi) <= 0|| Frame::CosTheta(bRec.wo) <= 0) {
            return Color3f(0.0f);
        }
        return albedo * kInvPI;
    }

    float Pdf(const BSDFQueryRecord &bRec) const override{
        if ( Frame::CosTheta(bRec.wi) <= 0 || Frame::CosTheta(bRec.wo) <= 0) {
            return 0.0f;
        }
        return kInvPI * Frame::CosTheta(bRec.wo);
    }

    Color3f Sample(BSDFQueryRecord &bRec, const Point2f &sample) const override {
        if (Frame::CosTheta(bRec.wi) <= 0) {
            return Color3f(0.0f);
        }
        bRec.wo = Warp::SquareToCosineHemisphere(sample);
        bRec.eta = 1.0f;
        return albedo;
    }

    bool IsDiffuse() const override{return true;}
};
