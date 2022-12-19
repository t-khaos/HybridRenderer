#pragma once

#include "Just/Common.h"
#include "Just/Texture/Texture2D.h"
#include "Just/Geometry/Mesh.h"
#include "Just/Core/MeshVertex.h"
#include "Just/Math/Transform.h"

struct AssetsManager
{
public:
    static AssetsManager *GetInstance()
    {
        static AssetsManager instance;
        return &instance;
    }
    AssetsManager(AssetsManager const &) = delete;
    void operator=(AssetsManager const &) = delete;
public:
    static Image *LoadImage(const std::string &path);
    static Texture2D *LoadTexture2D(const std::string &path);
    static Mesh *LoadMesh(const std::string &path, const Transform &transform = Transform{});
private:
    AssetsManager() = default;
};

//读取图片
Image *AssetsManager::LoadImage(const std::string &path)
{
    Image *image = new Image;
    stbi_set_flip_vertically_on_load(true);
    auto tmp = stbi_load(path.c_str(), &image->width, &image->height, &image->channel, 0);
    if (tmp == nullptr)
    {
        std::cerr << "Failed to load image: " << path << std::endl;
    }
    delete[] image->data;
    image->data = new RGBA32[image->width * image->height];
    for (int i = 0; i < image->width * image->height; i++)
    {
        image->data[i].r = tmp[i * image->channel + 0];
        image->data[i].g = image->channel >= 2 ? tmp[i * image->channel + 1] : 0;
        image->data[i].b = image->channel >= 3 ? tmp[i * image->channel + 2] : 0;
        image->data[i].a = image->channel >= 4 ? tmp[i * image->channel + 3] : 255;
    }
    image->format = ImageFormat(image->channel);
    stbi_image_free(tmp);
    return image;
}
//读取纹理
Texture2D *AssetsManager::LoadTexture2D(const std::string &path)
{
    return new Texture2D(LoadImage(path));
}
//读取obj文件
Mesh *AssetsManager::LoadMesh(const std::string &path, const Transform &transform)
{
    auto *mesh = new Mesh;
    std::ifstream fileStream(path);
    std::string line;
    std::unordered_map<MeshVertex, size_t, MeshVertexHash> meshVertexMap;
    std::vector<Point3f> vs;
    std::vector<Point2f> vts;
    std::vector<Vector3f> vns;
    std::vector<MeshVertex> meshVertices;
    while (std::getline(fileStream, line))
    {
        std::istringstream strStream(line);
        std::string prefix;
        strStream >> prefix;
        //解析数据
        if (prefix == "v")
        {
            Point4f v4f;
            strStream >> v4f.x >> v4f.y >> v4f.z;
            v4f.w = 1;
            auto v3f = transform(v4f);
            vs.push_back(v3f);
            mesh->bounds.Expand(v3f);
        }
        else if (prefix == "vn")
        {
            Vector3f vn;
            strStream >> vn.x >> vn.y >> vn.z;
            vns.push_back(Normalize(transform(vn)));
        }
        else if (prefix == "vt")
        {
            Point2f vt;
            strStream >> vt.x >> vt.y;
            vts.push_back(vt);
        }
        else if (prefix == "f")
        {
            std::string v1, v2, v3, v4;
            strStream >> v1 >> v2 >> v3 >> v4;
            MeshVertex meshTriangle[6];
            int vNums = 3;
            meshTriangle[0] = StringToMeshVertex(v1);
            meshTriangle[1] = StringToMeshVertex(v2);
            meshTriangle[2] = StringToMeshVertex(v3);
            if (!v4.empty())
            {
                vNums = 6;
                meshTriangle[3] = StringToMeshVertex(v1);
                meshTriangle[4] = StringToMeshVertex(v3);
                meshTriangle[5] = StringToMeshVertex(v4);
            }
            for (int i = 0; i < vNums; i++)
            {
                const MeshVertex &v = meshTriangle[i];
                auto it = meshVertexMap.find(v);
                if (it == meshVertexMap.end())
                {
                    meshVertexMap[v] = meshVertices.size();
                    mesh->indices.push_back(meshVertices.size());
                    meshVertices.push_back(v);
                }
                else
                {
                    mesh->indices.push_back(it->second);
                }
            }
        }
    }
    //读取顶点坐标
    mesh->positions.resize(meshVertices.size());
    for (size_t i = 0; i < meshVertices.size(); ++i)
    {
        mesh->positions[i] = vs.at(meshVertices[i].p - 1);
    }
    //读取法线
    if (!vns.empty())
    {
        mesh->normals.resize(meshVertices.size());
        for (size_t i = 0; i < meshVertices.size(); ++i)
        {
            mesh->normals[i] = vns.at(meshVertices[i].n - 1);
        }
    }
    //读取纹理坐标
    if (!vts.empty())
    {
        mesh->texcoords.resize(meshVertices.size());
        for (size_t i = 0; i < meshVertices.size(); ++i)
        {
            mesh->texcoords[i] = vts.at(meshVertices[i].uv - 1);
        }
    }
    return mesh;
}