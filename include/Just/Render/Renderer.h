#pragma once

#include "Just/Common.h"
#include "Just/Render/Scene.h"
#include "Just/Render/RenderContext.h"
#include "Just/Sampler/Sampler.h"

struct Renderer
{
public:
    Renderer(std::shared_ptr<Scene> scene,std::shared_ptr<RenderContext> context)
            : scene(scene), context(context) {};
    virtual ~Renderer() = default;
    virtual void Render() = 0;
protected:
    std::shared_ptr<Scene> scene;
    std::shared_ptr<RenderContext> context;
};

struct Rasterizer : public Renderer
{
public:
    Rasterizer(const std::shared_ptr<Scene> &scene, const std::shared_ptr<RenderContext> &context)
            : Renderer(scene, context) {};
    virtual ~Rasterizer() = default;
    virtual void Render() = 0;
private:
    virtual void DrawTriangle() = 0;
};

struct Tracer : public Renderer
{
public:
    explicit Tracer(const std::shared_ptr<Scene> &scene,
                    const std::shared_ptr<RenderContext> &context,
                    const std::shared_ptr<Sampler> &sampler)
            : Renderer(scene, context), sampler(sampler) {};
    virtual ~Tracer() = default;
    virtual void Render() = 0;
public:
    std::shared_ptr<Sampler> sampler;
    virtual Color3f Li(const Ray &ray) const = 0;
};