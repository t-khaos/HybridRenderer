#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Math/Color.h"

struct Texture
{
public:
    virtual Color3f Evaluate(float u, float v) const = 0;
    virtual ~Texture() = default;
};

struct ConstantTexture : public Texture
{
public:
    explicit ConstantTexture(const Color3f &value) : value(value) {}
    ~ConstantTexture() override = default;
    Color3f Evaluate(float u, float v) const override { return value; }
private:
    Color3f value;
};