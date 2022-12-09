#pragma once

#include "Just/Common.h"
#include "Just/Camera/Camera.h"
#include "Just/Math/Transform.h"

struct ProjectiveCamera : public Camera
{
    Transform cameraToScreen;
    Transform screenToRaster;
    Transform rasterToScreen;
    Transform rasterToCamera;
    ProjectiveCamera(const Point2i &resolution, const Transform &cameraToWorld, const Transform &cameraToScreen)
            : Camera(resolution, cameraToWorld), cameraToScreen(cameraToScreen)
    {
        float aspect = (float) resolution.x / (float) resolution.y;
        screenToRaster = Scale((float) resolution.x, (float) resolution.y, 1) *
                         Scale(-0.5f, -0.5f * aspect, 1.0f) *
                         Translate(Vector3f(-1.0f, -1.0f / aspect, 0.0f));
        rasterToScreen = Inverse(screenToRaster);
        rasterToCamera = Inverse(cameraToScreen) * rasterToScreen;
    }
    ~ProjectiveCamera() override = default;
    Transform GetProjective() const { return cameraToScreen; }
    Transform GetView() const { return Inverse(cameraToWorld); }
    Ray GenerateRay(const Point2f &sample) const override = 0;
};

struct PerspectiveCamera : public ProjectiveCamera
{
    PerspectiveCamera(const Point2i &resolution, const Transform &cameraToWorld, float fov, float near, float far)
            : ProjectiveCamera(resolution, cameraToWorld, Perspective(fov, near, far)) {}
    ~PerspectiveCamera() override = default;
    Ray GenerateRay(const Point2f &sample) const override
    {
        //相机空间中相机原点为(0,0,0)
        Point3f origin{0, 0, 0};
        //在栅格空间中采样的点转到相机空间
        Point3f pCamera = rasterToCamera(Point4f{sample, 0, 1});
        //相机空间下的射线转为世界空间
        return {cameraToWorld(Point4f(origin, 1)), cameraToWorld(Normalize(pCamera - origin))};
    }
};

struct OrthographicCamera : public ProjectiveCamera
{
    OrthographicCamera(const Point2i &resolution, const Transform &cameraToWorld, float near, float far)
            : ProjectiveCamera(resolution, cameraToWorld, Transform(Orthographic(near, far))) {}
    Ray GenerateRay(const Point2f &sample) const override
    {
        //在栅格空间中采样的点转到相机空间
        Point3f pCamera = rasterToCamera(Point3f{sample.x, sample.y, 0});
        //相机空间下的射线转为世界空间
        return {cameraToWorld(pCamera), cameraToWorld(Vector3f{0, 0, 1})};
    }
};
