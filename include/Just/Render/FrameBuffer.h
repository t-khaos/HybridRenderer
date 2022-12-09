#pragma once

#include "Just/Common.h"
#include "Just/Math/Color.h"
#include "Just/Geometry/Bounds.h"
#include "Just/Texture/Texture2D.h"

struct FrameBuffer
{
public:
    RGBA32 *colorBuffer;
    float *depthBuffer;
    Point2i res;
    Bounds2i screenRect;
    explicit FrameBuffer(const Point2i &res) :
            res{res},
            screenRect{0, 0, res.x - 1, res.y - 1}
    {
        colorBuffer = new RGBA32[res.x * res.y];
        depthBuffer = new float[res.x * res.y];
    }
    ~FrameBuffer()
    {
        delete[] colorBuffer;
        delete[] depthBuffer;
    }
};
