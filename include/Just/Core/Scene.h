#pragma once

#include "Just/Common.h"
#include "Just/Geometry/Mesh.h"
#include "Just/Core/Accel.h"
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
    std::vector<Ref<Mesh>> emitters;
    std::shared_ptr<Accel> accel;
};


void Scene::AddMesh(const std::shared_ptr<Mesh> &mesh)
{
    meshes.emplace_back(mesh);
    if(mesh->emitter!= nullptr){
        emitters.emplace_back(mesh);
    }
}

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