#pragma once

#include "Just/Common.h"
#include "Just/Geometry/Mesh.h"
#include "Just/Accel/Accel.h"
#include "Just/Accel/NaiveAccel.h"
#include "Just/Accel/BVHAccel.h"

struct Scene
{
public:
    explicit Scene(const std::shared_ptr<Accel> &accel) : accel(accel) {};
    void BuildAccel();
    void AddMesh(const std::shared_ptr<Mesh> &mesh);
    bool RayIntersect(const Ray &ray, HitRecord &record) const;
    bool RayIntersect(const Ray &ray) const;
    ~Scene() = default;
public:
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::shared_ptr<Accel> accel;
};

//添加三角形网格
void Scene::AddMesh(const std::shared_ptr<Mesh> &mesh)
{
    meshes.emplace_back(mesh);
}
//激活加速结构
void Scene::BuildAccel()
{
    accel->Reset();
    for (const auto &mesh: meshes)
    {
        accel->AddMesh(mesh);
    }
    accel->Build();
}
bool Scene::RayIntersect(const Ray &ray, HitRecord &record) const
{
    return accel->RayIntersect(ray, record, false);
}
bool Scene::RayIntersect(const Ray &ray) const
{
    HitRecord record;
    return accel->RayIntersect(ray, record, true);
}