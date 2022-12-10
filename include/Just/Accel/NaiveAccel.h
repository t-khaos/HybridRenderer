#pragma once

#include "Just/Common.h"
#include "Just/Core/Accel.h"

struct NaiveAccel : public Accel
{
public:
    explicit NaiveAccel(int nums = 1, int depth = 1) : Accel(nums, depth) {}
    ~NaiveAccel() override = default;
    virtual void Divide(size_t nodeIndex, std::vector<AccelNode> &children) override;
    virtual void Traverse(const Ray &ray, size_t nodeIndex, std::queue<size_t> &queue) const override;
    virtual void StaticCulling() override;
};

void NaiveAccel::Divide(size_t nodeIndex, std::vector<AccelNode> &children)
{
}
void NaiveAccel::Traverse(const Ray &ray, size_t nodeIndex, std::queue<size_t> &queue) const
{
}
void NaiveAccel::StaticCulling()
{
}

