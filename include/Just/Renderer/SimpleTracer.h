#pragma once

#include "Just/Common.h"
#include "Just/Core/Renderer.h"
#include "Just/Core/Camera.h"
#include "Just/Core/Sampler.h"

struct SimpleTracer : public Tracer
{
public:
    explicit SimpleTracer(const std::shared_ptr<Scene> &scene,
                          const std::shared_ptr<RenderContext> &context,
                          const std::shared_ptr<Sampler> &sampler)
            : Renderer(scene, context), Tracer(scene, context, sampler) {};
    ~SimpleTracer() override = default;
    virtual void Render() override;
public:
    virtual Color3f Li(const Ray &ray) const override;
};

void SimpleTracer::Render()
{
    Color3f radiance(0.0f);
    int width = context->camera->res.x;
    int height = context->camera->res.y;
#ifdef ENABLE_OPENMP
    //OpenMP多线程渲染
//#pragma omp parallel for schedule(dynamic) private(radiance)
#endif
    for (int y = 0; y < height; ++y)
    {
        //printf("\r%f\n", 100.0f * float(y) / float(film->resolution.y - 1));
        for (int x = 0; x < width; ++x)
        {
            radiance = Color3f(0.0f);
            for (int i = 0; i < sampler->spp; ++i)
            {
                //投射光线并累计颜色
                Ray ray = context->camera->GenerateRay(Point2f(float(x), float(y)));
                radiance += Li(ray);
            }
            radiance /= sampler->spp;
            int index = y * width + x;
            context->frameBuffer->colorBuffer[index] = Color3fToRGBA32(LinearToSRGB(radiance));
        }
    }
}
Color3f SimpleTracer::Li(const Ray &ray) const
{
    //射线相交测试
    HitRecord record;
    if (!scene->RayIntersect(ray, record))
    {
        return Color3f(0.0f);
    }
    //Vector3f normal = Abs(record.shadingFrame.n);
/*    auto diffuseTexture = context->GetTexture(0);
    auto diffuseColor = diffuseTexture->Evaluate(record.uv.x, record.uv.y);
    return diffuseColor;*/
    Point3f position{1.0f, 1.0f, 0.0f};
    Color3f energy = Color3f(100.0f);

    Vector3f L = position - record.point;//光源方向
    if (scene->RayIntersect(Ray(record.point + L * kEpsilon, L)))
    {
        return Color3f(0.0f);
    }
    auto normal = record.shadingFrame.n;
    auto L_N = Normalize(L);
    auto cosTheta = std::max(0.0f, Dot(normal, L_N));
    Color3f result = 0.25f * kInvPI * kInvPI * energy * 5.0f * cosTheta / LengthSquare(L);
    return result;
}