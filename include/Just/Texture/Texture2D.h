#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Texture/Image.h"

struct Texture2D
{
public:
    std::unique_ptr<Image> image;
    explicit Texture2D(Image *image) : image(image) {}
    int GetWidth() const { return image->width; }
    int GetHeight() const { return image->height; }
    RGBA32 SampleByNearest(const Point2f &coord) const;
    RGBA32 SampleByBilinear(const Point2f &coord) const;
};

inline RGBA32 Texture2D::SampleByNearest(const Point2f &coord) const
{
    float x = coord.x * (float) image->width + 0.5f;
    float y = coord.y * (float) image->height + 0.5f;
    uint32_t u = static_cast<uint32_t>(x);
    uint32_t v = static_cast<uint32_t>(y);
    return image->data[u + v * image->width];
}
inline RGBA32 Texture2D::SampleByBilinear(const Point2f &coord) const
{
    float x = coord.x * (float) image->width + 0.5f;
    float y = coord.y * (float) image->height + 0.5f;
    //整型坐标
    int x1 = std::clamp((int) x, 0, image->width - 1);
    int y1 = std::clamp((int) y, 0, image->height - 1);
    int x2 = std::clamp(x1 + 1, 0, image->width - 1);
    int y2 = std::clamp(y1 + 1, 0, image->height - 1);
    //权重
    float w00 = ((float) x2 - x) * ((float) y2 - y);
    float w01 = ((float) x2 - x) * (y - (float) y1);
    float w10 = (x - (float) x1) * ((float) y2 - y);
    float w11 = (x - (float) x1) * (y - (float) y1);
    //采样颜色
    Color3f u00 = RGBA32ToColor3f(image->data[x1 + y1 * image->width]);
    Color3f u10 = RGBA32ToColor3f(image->data[x2 + y1 * image->width]);
    Color3f u01 = RGBA32ToColor3f(image->data[x1 + y2 * image->width]);
    Color3f u11 = RGBA32ToColor3f(image->data[x2 + y2 * image->width]);
    //线性插值
    return Color3fToRGBA32(u00 * w00 + u10 * w10 + u01 * w01 + u11 * w11);
}

