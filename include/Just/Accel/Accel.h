#pragma once

#include "Just/Common.h"
#include "Just/Geometry/Mesh.h"
#include "Just/Geometry/Bounds.h"
#include "Just/Core/RenderContext.h"

enum class AccelType
{
    NaiveAccel = 0,
    BVHAccel = 1,
    OctreeAccel = 2,
};

struct AccelNode
{
    size_t child;
    Bounds3f bounds;
    std::vector<std::pair<size_t, size_t>> faceIndices;
    AccelNode() : bounds(), child(0) {}
    explicit AccelNode(const Bounds3f &bounds) : bounds(bounds), child(0) {}
    AccelNode(const Bounds3f &bounds, size_t size)
            : bounds(bounds), faceIndices(size), child(0) {}
};

struct Accel
{
public:
    Accel(int nums, int depth) : minNumFaces(nums), maxDepth(depth) {}
    virtual ~Accel() = default;
    void AddMesh(const std::shared_ptr<Mesh> &mesh);
    //通用部分
    void Reset();
    void Build();
    //光栅化部分
    virtual void StaticCulling() = 0;
    //光线追踪部分
    //射线相交测试
    bool RayIntersect(const Ray &ray, HitRecord &record, bool isShadow) const;
    bool RayIntersect(const Ray &ray, bool shadow) const;
    //划分子节点
    virtual void Divide(size_t nodeIndex, std::vector<AccelNode> &children) = 0;
    //遍历子节点
    virtual void Traverse(const Ray &ray, size_t nodeIndex, std::queue<size_t> &queue) const = 0;
public:
    std::vector<std::shared_ptr<Mesh>> meshes;
    //加速结构树
    std::vector<AccelNode> tree;
    //场景总包围盒
    Bounds3f bounds;
    //场景图元索引
    std::vector<std::pair<size_t, size_t>> faceIndices;
    int currDepth = 0;
    int leafCount = 1;
    int nodeCount = 1;
    int minNumFaces = 0;
    int maxDepth = 0;
};

void Accel::Reset()
{
    meshes.clear();
    meshes.shrink_to_fit();
    tree.clear();
    tree.shrink_to_fit();
    faceIndices.clear();
    faceIndices.shrink_to_fit();
    bounds = Bounds3f();
}
void Accel::AddMesh(const std::shared_ptr<Mesh> &mesh)
{
    //更新包围盒
    bounds.Expand(mesh->bounds);
    //更新顶点索引
    for (int i = 0; i < mesh->indices.size() / 3; i++)
    {
        faceIndices.emplace_back(meshes.size(), i);
    }
    meshes.push_back(mesh);
    std::cout << "[info]: success to add mesh." << "\n";
}
void Accel::Build()
{
    //计算场景包围盒
    std::cout << "[info]: building accel......" << "\n";
    //初始化根节点
    auto root = AccelNode(bounds, faceIndices.size());
    root.faceIndices = faceIndices;
    std::cout << "[mesh count]:" << meshes.size() << "\n";
    std::cout << "[vertex count]:" << faceIndices.size() << "\n";
    //初始化树
    tree = std::vector<AccelNode>();
    tree.emplace_back(root);
    //初始化辅助队列
    std::queue<size_t> q;
    q.push(0);
    //初始化子节点集合
    std::vector<AccelNode> children;
    //构建树
    while (!q.empty())
    {
        auto size = q.size(); //层次遍历
        for (int i = 0; i < size; ++i)
        {
            auto &node = tree[q.front()];
            //判断深度和图元数量是否超过符合限制
            if (node.faceIndices.size() > minNumFaces && currDepth < maxDepth)
            {
                //设置子节点起始索引
                node.child = tree.size();
                //检测是否可以分割当前节点的空间
                Divide(q.front(), children);
                --leafCount;
                //将分离的子节点加入树，索引入队
                for (auto &child: children)
                {
                    q.push(tree.size());
                    tree.emplace_back(child);
                    ++leafCount;
                    ++nodeCount;
                }
            }
            q.pop();
            children.clear();
            children.shrink_to_fit();
        }
        currDepth++;
    }
    //统计数据
    std::cout << "[tree depth]: " << currDepth << std::endl;
    std::cout << "[node count]: " << nodeCount << std::endl;
    std::cout << "[leaf count]: " << leafCount << std::endl;
}
bool Accel::RayIntersect(const Ray &ray, HitRecord &record, bool isShadow = false) const
{
    //初始化辅助队列
    std::queue<size_t> q;
    q.push(0);
    auto [m, f] = std::tuple<size_t, size_t>();
    bool isHit = false;
    //层次遍历树
    while (!q.empty())
    {
        auto size = q.size();
        for (int i = 0; i < size; ++i)
        {
            auto nodeIndex = q.front();
            q.pop();
            //包围盒相交测试
            if (!tree[nodeIndex].bounds.RayIntersect(ray))
            {
                continue;
            }
            //节点为叶子节点
            if (tree[nodeIndex].child == 0)
            {
                //遍历节点内图元进行相交测试
                for (auto [meshIndex, faceIndex]: tree[nodeIndex].faceIndices)
                {
                    if (meshes[meshIndex]->RayIntersect(faceIndex, ray, record))
                    {
                        //阴影测试击中直接返回
                        if (isShadow)
                        {
                            return true;
                        }
                        //记录交点信息
                        record.meshIndex = meshIndex;
                        f = faceIndex;
                        isHit = true;
                    }
                }
                //在叶子节点内的图元击中则终止遍历
                if (isHit)
                {
                    break;
                }
            }
            else
            {
                //对非叶子节点继续遍历
                Traverse(ray, nodeIndex, q);
            }
        }
    }

    //阴影射线未击中物体
    if (isShadow)
    {
        return false;
    }

    //击中物体，插值计算
    if (isHit)
    {
        //计算重心坐标
        Vector3f bary;
        bary.x = 1 - record.uv.x - record.uv.y;
        bary.y = record.uv.x;
        bary.z = record.uv.y;

        //网格缓冲引用
        const auto &recordMesh = meshes[record.meshIndex];
        const auto &V = recordMesh->positions;
        const auto &N = recordMesh->normals;
        const auto &UV = recordMesh->texcoords;
        //获取顶点索引
        const auto [idx0, idx1, idx2] = recordMesh->GetTriangle(f);

        //插值顶点
        record.point = bary.x * V[idx0] + bary.y * V[idx1] + bary.z * V[idx2];

        //插值纹理坐标
        if (!UV.empty())
        {
            record.uv = bary.x * UV[idx0] + bary.y * UV[idx1] + bary.z * UV[idx2];
        }

        //面法线坐标系
        record.geometryFrame = Frame(Normalize(Cross(V[idx1] - V[idx0], V[idx2] - V[idx0])));

        //插值法线坐标系
        if (!recordMesh->normals.empty())
        {
            record.shadingFrame = Frame(Normalize(bary.x * N[idx0] + bary.y * N[idx1] + bary.z * N[idx2]));
        }
        else
        {
            record.shadingFrame = record.geometryFrame;
        }
    }
    return isHit;
}
bool Accel::RayIntersect(const Ray &ray, bool shadow = true) const
{
    HitRecord unused;
    return RayIntersect(ray, unused, shadow);
}


