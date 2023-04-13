#pragma once

#include "Just/Common.h"
#include "Just/Core/Renderer.h"
#include "Just/Core/Camera.h"
#include "Just/Core/Sampler.h"
#include "Just/Core/BSDFQuerayRecord.h"

struct SimpleTracer : public Tracer {
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

void SimpleTracer::Render() {

}