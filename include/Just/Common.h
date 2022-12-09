#pragma once

#include <iostream>
#include <memory>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <sstream>
#include <stbi/stb_image.h>
#include <stbi/stb_image_write.h>

float kPI = 3.14159265358979323846f;
float kInvPI = 0.31830988618379067154f;
float kInv2PI = 0.15915494309189533577f;
float kInv4PI = 0.07957747154594766788f;
float kSqrt2 = 1.41421356237309504880f;
float kEpsilon = 1e-4f;
//数学计算
inline float Radians(float deg)
{
    return kPI / 180.0f * deg;
}
inline float Degrees(float rad)
{
    return 180.0f / kPI * rad;
}
inline void SaveImageToBMP(const std::string &path, int width, int height, int channel, const void *data)
{
    stbi_write_bmp(path.c_str(), width, height, channel, data);
}

//解析字符串
template<typename T>
inline T StringToNumeric(const std::string& str)
{
    char* end = nullptr;
    T result = static_cast<T>(strtoul(str.c_str(), &end, 10));
    return result;
}
inline std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiter, bool includeEmpty)
{
    std::string::size_type currPos = 0;
    std::string::size_type nextPos = str.find(delimiter, currPos);
    std::vector<std::string> tokens;
    while (currPos != std::string::npos)
    {
        if (nextPos != currPos || includeEmpty)
        {
            tokens.push_back(str.substr(currPos, nextPos - currPos));
        }
        currPos = nextPos;
        if (currPos != std::string::npos)
        {
            currPos++;
            nextPos = str.find_first_of(delimiter, currPos);
        }
    }
    return tokens;
}
