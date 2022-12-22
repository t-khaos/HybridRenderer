#pragma once

#include "Just/Common.h"

template<typename T>
struct Color3
{
    T r, g, b;
    //构造函数
    Color3() : r(T()), g(T()), b(T()) {}
    explicit Color3(T val) : r(val), g(val), b(val) {}
    Color3(T r, T g, T b) : r(r), g(g), b(b) {}
    //索引
    T &operator[](size_t i) { return (&r)[i]; }
    T operator[](size_t i) const { return (&r)[i]; }
};

template<typename T>
struct Color4
{
    T r, g, b, a;
    //构造函数
    Color4() : r(T()), g(T()), b(T()), a(255) {}
    explicit Color4(T val) : r(val), g(val), b(val), a(val) {}
    Color4(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}
    Color4(const Color3<T> &color, T a) : r(color.r), g(color.g), b(color.b), a(a) {}
    //索引
    T &operator[](size_t i) { return (&r)[i]; }
    T operator[](size_t i) const { return (&r)[i]; }
};

using Color3f = Color3<float>;
using RGB8 = Color3<uint8_t>;
using Color4f = Color4<float>;
using RGBA32 = Color4<uint8_t>;
//Color3<T>
//c1+c2
template<typename T>
inline Color3<T> operator+(const Color3<T> &c1, const Color3<T> &c2)
{
    return {c1.r + c2.r, c1.g + c2.g, c1.b + c2.b};
}
//c1+=c2
template<typename T>
inline Color3<T> &operator+=(Color3<T> &c1, const Color3<T> &c2)
{
    c1.r += c2.r;
    c1.g += c2.g;
    c1.b += c2.b;
    return c1;
}
//c1-c2
template<typename T>
inline Color3<T> operator-(const Color3<T> &c1, const Color3<T> &c2)
{
    return {c1.r - c2.r, c1.g - c2.g, c1.b - c2.b};
}
//c1-=c2
template<typename T>
inline Color3<T> &operator-=(Color3<T> &c1, const Color3<T> &c2)
{
    c1.r -= c2.r;
    c1.g -= c2.g;
    c1.b -= c2.b;
    return c1;
}
//c1*c2
template<typename T>
inline Color3<T> operator*(const Color3<T> &c1, const Color3<T> &c2)
{
    return {c1.r * c2.r, c1.g * c2.g, c1.b * c2.b};
}
//c1*=c2
template<typename T>
inline Color3<T> &operator*=(Color3<T> &c1, const Color3<T> &c2)
{
    c1.r *= c2.r;
    c1.g *= c2.g;
    c1.b *= c2.b;
    return c1;
}
//c*n
template<typename T>
inline Color3<T> operator*(const Color3<T> &c, T val)
{
    return {c.r * val, c.g * val, c.b * val};
}
//n*c
template<typename T>
inline Color3<T> operator*(T val, const Color3<T> &c)
{
    return {val * c.r, val * c.g, val * c.b};
}
//c*=n
template<typename T>
inline Color3<T> &operator*=(Color3<T> &c, T val)
{
    c.r *= val;
    c.g *= val;
    c.b *= val;
    return c;
}
//c1/c2
template<typename T>
inline Color3<T> operator/(const Color3<T> &c1, const Color3<T> &c2)
{
    return {c1.r / c2.r, c1.g / c2.g, c1.b / c2.b};
}
//c1/=c2
template<typename T>
inline Color3<T> &operator/=(Color3<T> &c1, const Color3<T> &c2)
{
    c1.r /= c2.r;
    c1.g /= c2.g;
    c1.b /= c2.b;
    return c1;
}
//c/n
template<typename T, typename U>
inline Color3<T> operator/(const Color3<T> &c, U val)
{
    return {static_cast<T>(c.r / val), static_cast<T>(c.g / val), static_cast<T>(c.b / val)};
}
//c/=n
template<typename T, typename U>
inline Color3<T> &operator/=(Color3<T> &c, U val)
{
    c = c / val;
    return c;
}
//Color4<T>
//c1+c2
template<typename T>
inline Color4<T> operator+(const Color4<T> &c1, const Color4<T> &c2)
{
    return {c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a};
}
//c1+=c2
template<typename T>
inline Color4<T> &operator+=(Color4<T> &c1, const Color4<T> &c2)
{
    c1.r += c2.r;
    c1.g += c2.g;
    c1.b += c2.b;
    c1.a += c2.a;
    return c1;
}
//c1-c2
template<typename T>
inline Color4<T> operator-(const Color4<T> &c1, const Color4<T> &c2)
{
    return {c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a};
}
//c1-=c2
template<typename T>
inline Color4<T> &operator-=(Color4<T> &c1, const Color4<T> &c2)
{
    c1.r -= c2.r;
    c1.g -= c2.g;
    c1.b -= c2.b;
    c1.a -= c2.a;
    return c1;
}
//c1*c2
template<typename T>
inline Color4<T> operator*(const Color4<T> &c1, const Color4<T> &c2)
{
    return {c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a};
}
//c1*=c2
template<typename T>
inline Color4<T> &operator*=(Color4<T> &c1, const Color4<T> &c2)
{
    c1.r *= c2.r;
    c1.g *= c2.g;
    c1.b *= c2.b;
    c1.a *= c2.a;
    return c1;
}
//c*n
template<typename T, typename U>
inline Color4<T> operator*(const Color4<T> &c, U val)
{
    return {static_cast<T>(c.r * val), static_cast<T>(c.g * val), static_cast<T>(c.b * val), c.a};
}
//n*c
template<typename T>
inline Color4<T> operator*(T val, const Color4<T> &c)
{
    return {val * c.r, val * c.g, val * c.b, val * c.a};
}
//c*=n
template<typename T>
inline Color4<T> &operator*=(Color4<T> &c, T val)
{
    c.r *= val;
    c.g *= val;
    c.b *= val;
    c.a *= val;
    return c;
}
//c1/c2
template<typename T>
inline Color4<T> operator/(const Color4<T> &c1, const Color4<T> &c2)
{
    return {c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a};
}
//c1/=c2
template<typename T>
inline Color4<T> &operator/=(Color4<T> &c1, const Color4<T> &c2)
{
    c1.r /= c2.r;
    c1.g /= c2.g;
    c1.b /= c2.b;
    c1.a /= c2.a;
    return c1;
}
//c/n
template<typename T>
inline Color4<T> operator/(const Color4<T> &c, T val)
{
    return {c.r / val, c.g / val, c.b / val, c.a / val};
}
//c/=n
template<typename T>
inline Color4<T> &operator/=(Color4<T> &c, T val)
{
    c.r /= val;
    c.g /= val;
    c.b /= val;
    c.a /= val;
    return c;
}

//convert

//FloatToUInt8
inline uint8_t FloatToUInt8(float val)
{
    return (uint8_t) (val * 255);
}
//UInt8ToFloat
inline float UInt8ToFloat(uint8_t val)
{
    return (float) val / 255;
}
//Color3fToRGBA32
inline RGBA32 Color3fToRGBA32(const Color3f &c)
{
    return {FloatToUInt8(c.r), FloatToUInt8(c.g), FloatToUInt8(c.b), 255};
}
inline Color3f RGBA32ToColor3f(const RGBA32 &c)
{
    return {UInt8ToFloat(c.r), UInt8ToFloat(c.g), UInt8ToFloat(c.b)};
}
//output
template<typename T>
inline std::ostream &operator<<(std::ostream &os, const Color3<T> &c)
{
    return os << "<" << c.r << " " << c.g << " " << c.b << ">" << std::endl;
}
//gamma correction
inline float LinearRGBToSRGB(float value)
{
    if (value <= 0.0031308f)
        return 12.92f * value;
    else
        return (1.0f + 0.055f)
               * std::pow(value, 1.0f / 2.4f) - 0.055f;
}
inline float SRGBToLinearRGB(float value)
{
    if (value <= 0.04045f)
        return value / 12.92f;
    else
        return std::pow((value + 0.055f) / (1.0f + 0.055f), 2.4f);
}
inline Color3f LinearToSRGB(const Color3f &c)
{
    return {LinearRGBToSRGB(c.r), LinearRGBToSRGB(c.g), LinearRGBToSRGB(c.b)};
}
inline Color3f SRGBToLinear(const Color3f &c)
{
    return {SRGBToLinearRGB(c.r), SRGBToLinearRGB(c.g), SRGBToLinearRGB(c.b)};
}