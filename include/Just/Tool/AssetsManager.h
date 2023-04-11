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
private:
    AssetsManager() = default;
};
inline void SaveImageToPNG(const std::string &path, int width, int height, int channel, const void *data)
{
    stbi_write_png(path.c_str(), width, height, channel, data, channel*width);
}
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