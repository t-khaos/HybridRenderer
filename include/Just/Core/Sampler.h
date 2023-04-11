#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Tool/Random.h"

struct Sampler
{
    int spp;
    int index;
    RNG rng;
    explicit Sampler(int spp) : spp(spp), index(0) {}
    virtual ~Sampler() = default;
    virtual std::tuple<float, float> Sample(float i, float j) = 0;
    virtual float Next1D();
    virtual Point2f Next2D();
    virtual void StartPixel();
    virtual bool StartNextPixel();
};

void Sampler::StartPixel()
{
    index = 0;
}
bool Sampler::StartNextPixel()
{
    return ++index < spp;
}
float Sampler::Next1D()
{
    return rng.UniformFloat();
}
Point2f Sampler::Next2D()
{
    return {rng.UniformFloat(), rng.UniformFloat()};
}