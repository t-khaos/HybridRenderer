#pragma once

#include "Just/Common.h"
#include "Just/Core/Renderer.h"
#include "Just/Math/Color.h"

struct SimpleRasterizer : public Rasterizer {
public:
    SimpleRasterizer(const std::shared_ptr<Scene> &scene, const std::shared_ptr<RenderContext> &context)
            : Renderer(scene, context), Rasterizer(scene, context) {};

    ~SimpleRasterizer() override = default;

    virtual void Render() override;

private:
    virtual void DrawTriangle(RasterVertex *triangle) override;
};

void SimpleRasterizer::Render() {
    //遍历所有三角形网格
    for (const auto &mesh: scene->accel->meshes) {
        RasterVertex triangle[3];
#ifdef ENABLE_OPENMP
        //OpenMP多线程渲染
#pragma omp parallel for schedule(dynamic) private(triangle)
#endif
        //遍历网格所有三角形
        for (int i = 0; i < mesh->indices.size(); i += 3) {
            auto idx0 = mesh->indices[i + 0];
            auto idx1 = mesh->indices[i + 1];
            auto idx2 = mesh->indices[i + 2];

            //更新三角形顶点信息
            for (int j = 0; j < 3; j++) {
                auto idx = mesh->indices[i + j];
                triangle[j].pos = mesh->positions[idx];
                triangle[j].pos4 = Point4f{mesh->positions[idx], 1};
                if (mesh->texcoords.empty()) {
                    triangle[j].texcoord = Vector2f(0, 0);
                } else {
                    triangle[j].texcoord = mesh->texcoords[idx];
                }
            }

            auto faceNormal = Normalize(
                    Cross((triangle[1].pos - triangle[0].pos), (triangle[2].pos - triangle[0].pos)));
            for (int j = 0; j < 3; j++) {
                auto idx = mesh->indices[i + j];
                if (mesh->normals.empty()) {
                    triangle[j].normal = faceNormal;
                } else {
                    triangle[j].normal = mesh->normals[idx];
                }
            }

            //绘制当前三角形
            DrawTriangle(triangle);
        }
    }
}

void SimpleRasterizer::DrawTriangle(RasterVertex *triangle) {
    Bounds2i rect;
    //几何阶段
    for (int i = 0; i < 3; ++i) {
        auto &vertex = triangle[i];
        //vertex shader
        {
            auto &MVP = context->GetUniform<Matrix4f>("MVP");
            vertex.pos4 = MVP * vertex.pos4;
        }
        float w = vertex.pos4.w;
        //CVV 剔除
        if (w == 0.0f) return;
        if (vertex.pos4.z < 0.0f || vertex.pos4.z > w) return;
        if (vertex.pos4.x < -w || vertex.pos4.x > w) return;
        if (vertex.pos4.y < -w || vertex.pos4.y > w) return;

        //w的倒数
        vertex.rhw = 1.0f / w;
        //透视除法
        vertex.pos4 *= vertex.rhw;

        //屏幕映射
        vertex.pos2f = Point2f{context->camera->screenToRaster(vertex.pos4)};
        //四舍五入
        vertex.pos2i = Point2i{(int) (vertex.pos2f.x + 0.5f), (int) (vertex.pos2f.y + 0.5f)};
        //vertex.pos2i = Point2i{(int) std::round(vertex.pos2f.origin), (int) std::round(vertex.pos2f.target)};


        //设置三角形矩形范围
        rect.Expand(vertex.pos2i);
    }

    //限制在屏幕范围内
    rect.Clamp(context->frameBuffer->screenRect);

    //背面剔除
    if (Cross(triangle[1].pos4 - triangle[0].pos4,
              triangle[2].pos4 - triangle[0].pos4).z >= 0)
        return;
    //光栅化阶段
    //OpenMP多线程渲染
    for (int y = rect.pMin.y; y <= rect.pMax.y; y++) {
        for (int x = rect.pMin.x; x <= rect.pMax.x; x++) {
            //计算当前像素的重心坐标
            auto [alpha, beta, gamma] = CalcBarycentric(triangle, (float) x, (float) y);

            //检查点是否在三角形内
            if (beta < 0 || gamma < 0 || gamma + beta > 1.0f + kEpsilon) continue;

            //插值深度 w=z的倒数
            float rhw = alpha * triangle[0].rhw +
                        beta * triangle[1].rhw +
                        gamma * triangle[2].rhw;

            //early-z
            int index = x + y * context->frameBuffer->res.x;
            if (rhw < context->frameBuffer->depthBuffer[index]) continue;
            context->frameBuffer->depthBuffer[index] = rhw;

            //透视插值校正
            float w = 1.0f / ((rhw == 0.0f) ? 1.0f : rhw);
            alpha = alpha * w * triangle[0].rhw;
            beta = beta * w * triangle[1].rhw;
            gamma = gamma * w * triangle[2].rhw;

            //插值纹理坐标
            auto texcoord = alpha * triangle[0].texcoord + beta * triangle[1].texcoord +
                            gamma * triangle[2].
                                    texcoord;
            //插值法线
            auto normal = alpha * triangle[0].normal + beta * triangle[1].normal + gamma * triangle[2].normal;


            //片元着色
            Color3f fragColor;
            {
/*                const auto &diffuseMap = context->GetTexture(0);
                fragColor = diffuseMap->Evaluate(texcoord.origin, texcoord.target);*/


                fragColor = 0.5f * (Color3f{normal.x, normal.y, normal.z} + Color3f(1, 1, 1));
            }
            context->frameBuffer->colorBuffer[index] = Color3fToRGBA32(LinearToSRGB(fragColor));
        }
    }
}