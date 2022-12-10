#pragma once

#include "Just/Common.h"
#include "Just/Core/Sampler.h"

struct TrapezoidalSampler : public Sampler
{
    explicit TrapezoidalSampler(int spp) : Sampler(spp), subIndex(0) {}
    ~TrapezoidalSampler() override = default;
    int subIndex;
    void StartPixel() override
    {
        Sampler::StartPixel();
        subIndex = 0;
    }
    bool StartNextPixel() override
    {
        //更新子像素索引和总索引
        if (subIndex >= 4)
        {
            subIndex = 0;
            index++;
        }
        subIndex++;
        return index <= spp;
    }
    std::tuple<float, float> Sample(float i, float j) override
    {
        subIndex++;

        //子像素位置
        auto sx = index % 2;
        auto sy = index / 2;

        //梯形分布
        float r1 = 2 * rng.UniformFloat();
        float r2 = 2 * rng.UniformFloat();
        float dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
        float dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
        return {i + (dx + float(sx) + 0.5f) / 2.0f, j + (dy + float(sy) + 0.5f) / 2.0f};
    }
};