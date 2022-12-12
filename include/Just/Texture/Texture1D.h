#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Texture/Image.h"
#include "Just/Core/Texture.h"

struct Texture1D : public Texture
{
public:
    std::unique_ptr<Image> image;
    explicit Texture1D(Image *image) : image(image) {}
    virtual ~Texture1D() override = default;
    virtual Color3f Evaluate(float u, float v) const override;
    int GetWidth() const { return image->width; }
    int GetHeight() const { return image->width; }
};

inline Color3f Texture1D::Evaluate(float u, float v) const
{
    float x = u * (float) image->width + 0.5f;
    uint32_t s = static_cast<uint32_t>(x);
    return RGBA32ToColor3f(image->data[s]);
}