#pragma once

#include <memory>

#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include <random>
#include <limits>
#include <fstream>


//定义
#define ENABLE_OPENMP //开启OpenMP
//全局常量
constexpr float kPI = 3.14159265358979323846f;
constexpr float kInvPI = 0.31830988618379067154f;
constexpr float kInv2PI = 0.15915494309189533577f;
constexpr float kInv4PI = 0.07957747154594766788f;
constexpr float kSqrt2 = 1.41421356237309504880f;
constexpr float kEpsilon = 1e-4f;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

namespace Path{
    inline std::filesystem::path ProjectDir() {
        std::filesystem::path projectPath = std::filesystem::current_path();
        while (projectPath.filename() != "Ares") {
            projectPath = projectPath.parent_path();
        }
        return projectPath;
    }
}


//数学计算
inline float Radians(float deg) { return kPI / 180.0f * deg; }
inline float Degrees(float rad) { return 180.0f / kPI * rad; }

//解析字符串
template<typename T>
inline T StringToNumeric(const std::string &str)
{
    char *end = nullptr;
    T result = static_cast<T>(strtoul(str.c_str(), &end, 10));
    return result;
}
//解析obj模型字符串
inline std::vector<std::string> Tokenize(const std::string &str, const std::string &delimiter, bool includeEmpty)
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
