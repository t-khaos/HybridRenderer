#pragma once

#include "Just/Common.h"
#include "Just/Math/Vector.h"
#include "Just/Math/Color.h"

enum class ImageFormat
{
    RGB24 = 3,
    RGBA32 = 4
};

struct Image
{
    RGBA32 *data = nullptr;
    int width, height, channel;
    ImageFormat format;
    Image() :
            width{}, height{}, channel{},
            format{ImageFormat::RGBA32} {};
    explicit Image(const Point2i &resolution, ImageFormat format = ImageFormat::RGBA32)
            : width{resolution.x}, height{resolution.y}, channel{(int)format},
              format(ImageFormat::RGBA32)
    {
        data = new RGBA32[width * height];
    }
    Image(const Image &image) : width(image.width), height(image.height), channel(image.channel),
                                format(image.format)
    {
        data = new RGBA32[width * height];
        memcpy(data, image.data, width * height * sizeof(RGBA32));
    }
    Image &operator=(const Image &image)
    {
        if (this == &image)
        {
            return *this;
        }
        width = image.width;
        height = image.height;
        channel = image.channel;
        format = image.format;
        data = new RGBA32[width * height];
        memcpy(data, image.data, width * height * sizeof(RGBA32));
        return *this;
    }
    ~Image()
    {
        delete[] data;
    }
};
