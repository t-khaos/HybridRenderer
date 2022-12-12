#pragma once

#include "Just/Common.h"
#include "Just/Core/FrameBuffer.h"
#include "Just/Core/RasterVertex.h"

struct RenderContext
{
public:
    std::shared_ptr<FrameBuffer> frameBuffer;
    std::shared_ptr<ProjectiveCamera> camera;
    explicit RenderContext(const std::shared_ptr<ProjectiveCamera> &camera, const std::shared_ptr<FrameBuffer> &frameBuffer)
            : camera(camera), frameBuffer(frameBuffer) {};
    ~RenderContext() = default;
    std::shared_ptr<Texture> GetTexture(int slot) const { return textures.at(slot); }
    void AddTexture(int slot, const std::shared_ptr<Texture> &texture) { textures.emplace_back(texture); }
    template<typename T>
    const T &GetUniform(const std::string &name) const;
    template<typename T>
    void SetUniform(const std::string &name, T value);
private:
    std::vector<std::shared_ptr<Texture>> textures;
    std::unordered_map<std::string, float> f32map;
    std::unordered_map<std::string, Vector2f> vec2map;
    std::unordered_map<std::string, Vector3f> vec3map;
    std::unordered_map<std::string, Point2f> point2map;
    std::unordered_map<std::string, Point3f> point3map;
    std::unordered_map<std::string, Matrix4f> mat4map;
};

template<typename T>
inline const T &RenderContext::GetUniform(const std::string &name) const
{
    if constexpr (std::is_same_v<T, float>)
        return f32map.at(name);
    else if constexpr (std::is_same_v<T, Vector2f>)
        return vec2map.at(name);
    else if constexpr (std::is_same_v<T, Vector3f>)
        return vec3map.at(name);
    else if constexpr (std::is_same_v<T, Point2f>)
        return point2map.at(name);
    else if constexpr (std::is_same_v<T, Point3f>)
        return point3map.at(name);
    else if constexpr (std::is_same_v<T, Matrix4f>)
        return mat4map.at(name);
    else
        return T();
}
template<typename T>
inline void RenderContext::SetUniform(const std::string &name, T value)
{
    if constexpr (std::is_same_v<T, float>)
        f32map[name] = value;
    else if constexpr (std::is_same_v<T, Vector2f>)
        vec2map[name] = value;
    else if constexpr (std::is_same_v<T, Vector3f>)
        vec3map[name] = value;
    else if constexpr (std::is_same_v<T, Point2f>)
        point2map[name] = value;
    else if constexpr (std::is_same_v<T, Point3f>)
        point3map[name] = value;
    else if constexpr (std::is_same_v<T, Matrix4f>)
        mat4map[name] = value;
}
