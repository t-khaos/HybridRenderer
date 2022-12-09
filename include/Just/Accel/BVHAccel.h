#pragma once

#include "Just/Common.h"
#include "Just/Accel/Accel.h"

struct BVHAccel : public Accel
{
private:
    const int kNumBuckets = 10;
public:
    explicit BVHAccel(int nums = 16, int depth = 32) : Accel(nums, depth) {}
    ~BVHAccel() override = default;
    virtual void Divide(size_t nodeIndex, std::vector<AccelNode> &children) override;
    virtual void Traverse(const Ray &ray, size_t nodeIndex, std::queue<size_t> &queue) const override;
    virtual void StaticCulling() override;
};

void BVHAccel::Divide(size_t nodeIndex, std::vector<AccelNode> &children)
{
    auto& node = tree[nodeIndex];
    //在最长维度排序
    size_t axis = node.bounds.MajorAxis();
    meshes[faceIndices[0].first]->GetFaceBounds(faceIndices[0].second).Centroid()[axis];
    std::sort(
            node.faceIndices.begin(),
            node.faceIndices.end(),
            [this, axis](std::pair<size_t , size_t> left, std::pair<size_t, size_t> right) {
                return meshes[left.first]->GetFaceBounds(left.second).Centroid()[axis] <
                       meshes[right.first]->GetFaceBounds(right.second).Centroid()[axis];
            }
    );

    //SAH方法
    //分桶
    float minCost = std::numeric_limits<float>::infinity();
    AccelNode leftNode, rightNode;
    std::vector<std::pair<size_t, size_t>> leftIndexes, rightIndexes;
    for (int i = 1; i < kNumBuckets; i++) {
        auto begin = node.faceIndices.begin();
        auto mid = node.faceIndices.begin() + (static_cast<int>(node.faceIndices.size()) * i / kNumBuckets);
        auto end = node.faceIndices.end();
        leftIndexes = std::vector<std::pair<size_t, size_t>>(begin, mid);
        rightIndexes = std::vector<std::pair<size_t, size_t>>(mid, end);

        //合并左右包围盒
        Bounds3f leftBounds, rightBounds;
        for (auto [meshIndex, faceIndex]: leftIndexes) {
            leftBounds.Expand(meshes[meshIndex]->GetFaceBounds(faceIndex));
        }
        for (auto [meshIndex, faceIndex]: rightIndexes) {
            rightBounds.Expand(meshes[meshIndex]->GetFaceBounds(faceIndex));
        }

        //计算成本
        float leftSA = leftBounds.SurfaceArea();
        float rightSA = rightBounds.SurfaceArea();
        float SA = node.bounds.SurfaceArea();
        float cost = 0.125f +
                     static_cast<float>(leftIndexes.size()) * leftSA / SA +
                     static_cast<float>(rightIndexes.size()) * rightSA / SA;

        //选取成本最小的分桶方案
        if (cost < minCost) {
            minCost = cost;
            leftNode.bounds = leftBounds;
            leftNode.faceIndices = leftIndexes;
            rightNode.bounds = rightBounds;
            rightNode.faceIndices = rightIndexes;
        }
    }

    children.emplace_back(leftNode);
    children.emplace_back(rightNode);
}
void BVHAccel::Traverse(const Ray &ray, size_t nodeIndex, std::queue<size_t> &queue) const
{
    //子节点入队
    queue.push(tree[nodeIndex].child);
    queue.push(tree[nodeIndex].child + 1);
}
void BVHAccel::StaticCulling()
{
}

