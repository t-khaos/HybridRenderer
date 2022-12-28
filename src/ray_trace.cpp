
#include "Just/Tool/AssetsManager.h"
#include "Just/Camera/ProjectiveCamera.h"
#include "Just/Core//RenderContext.h"
#include "Just/Core/Scene.h"
#include "Just/Renderer/SimpleTracer.h"
#include "Just/Tool/Timer.h"
#include "Just/Sampler/TrapezoidalSampler .h"

int main()
{
    //数据
    //==================================================================================================
    Point3f target(0.0f, 0.0f, 0.0f);
    Point3f origin(0.0f, 0.0f, 1.0f);
    Vector3f up(0, 1, 0);
    Point2i res(768, 768);
    float fov = 45;
    float zNear = 1e-4f;
    float zFar = 1e4f;
    int spp = 1;
    auto modelTransform =  Translate(Vector3f(0, 0, -3));
    //资源
    //==================================================================================================
    auto texture_diffuse = std::shared_ptr<Texture2D>(AssetsManager::LoadTexture2D("res/test_cube_diffuse.tga"));
    auto mesh_african = std::shared_ptr<Mesh>(AssetsManager::LoadMesh("res/model_african.obj", modelTransform));
    //相机
    //==================================================================================================
    auto camera = std::make_shared<PerspectiveCamera>(res, Inverse(LookAt(origin, target, up)), fov, zNear, zFar);
    //帧缓冲
    //==================================================================================================
    auto frameBuffer = std::make_shared<FrameBuffer>(res);
    //上下文
    //==================================================================================================
    auto context = std::make_shared<RenderContext>(camera, frameBuffer);
    context->AddTexture(0, texture_diffuse);
    //采样器
    //==================================================================================================
    auto sampler = std::make_shared<TrapezoidalSampler>(spp);

    //构建场景
    //==================================================================================================
    Timer timer;
    timer.Begin();
    auto accel = std::make_shared<BVHAccel>();
    auto scene = std::make_shared<Scene>(accel);
    scene->AddMesh(mesh_african);
    scene->BuildAccel();
    timer.End();
    std::cout << "[build time]: " << timer.time << "ms" << std::endl;
    //渲染场景
    //==================================================================================================
    timer.Begin();
    auto tracer = std::make_shared<SimpleTracer>(scene, context, sampler);
    tracer->Render();
    timer.End();
    std::cout << "[render time]: " << timer.time << "ms" << std::endl;
    std::cout << "[FPS]: " << 1000.0f / timer.time << std::endl;
    //==================================================================================================
    //保存
    SaveImageToBMP("output/AF_ray_trace.bmp", res.x, res.y, 4, context->frameBuffer->colorBuffer);
    return 0;
}
