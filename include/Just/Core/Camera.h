#pragma once

#include "Just/Common.h"
#include "Just/Math/Transform.h"

struct Camera
{
    Transform cameraToWorld;
    Point2i res;
    Camera(const Point2i &resolution, const Transform &cameraToWorld)
            : res(resolution), cameraToWorld(cameraToWorld) {}
    virtual ~Camera() = default;
    virtual Ray GenerateRay(const Point2f &sample) const = 0;
};
