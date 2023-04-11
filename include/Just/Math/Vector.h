#pragma once

#include "Just/Common.h"
#include "Just/Math/Matrix.h"

//预声明
template<typename T>
struct Matrix<1, 2, T>;
template<typename T>
struct Matrix<1, 3, T>;
template<typename T>
struct Matrix<1, 4, T>;

template<size_t N, typename T>
struct Matrix<1, N, T>
{
    union
    {
        T matrix[0][N];
        T vec[N];
    };
    Matrix() { for (size_t i = 0; i < N; i++)vec[i] = 0; };
    explicit Matrix(T val) { for (size_t i = 0; i < N; i++); }
    Matrix(const Matrix<1, N, T> &src) { for (size_t i = 0; i < N; i++)matrix[0][i] = src.matrix[0][i]; }
    ~Matrix() = default;
    const T &operator[](size_t index) const { return vec[index]; }
    T &operator[](size_t index) { return vec[index]; }
};

template<typename T>
struct Matrix<1, 2, T>
{
    union
    {
        T matrix[1][2];
        T vec[2];
        struct
        {
            T x, y;
        };
    };
    Matrix() : x(0), y(0) {}
    explicit Matrix(T val) : x(val), y(val) {}
    Matrix(T x, T y) : x(x), y(y) {}
    Matrix(const Matrix<1, 2, T> &src) : x(src.x), y(src.y) {}
    explicit Matrix(const Matrix<1, 3, T> &src) : x(src.x), y(src.y) {}
    explicit Matrix(const Matrix<1, 4, T> &src) : x(src.x), y(src.y) {}
    ~Matrix() = default;
    const T &operator[](size_t index) const { return vec[index]; }
    T &operator[](size_t index) { return vec[index]; }
};

template<typename T>
struct Matrix<1, 3, T>
{
    union
    {
        T matrix[1][3];
        T vec[3];
        struct
        {
            T x, y, z;
        };
    };
    Matrix() : x(0), y(0), z(0) {}
    explicit Matrix(T val) : x(val), y(val), z(val) {}
    Matrix(T x, T y, T z) : x(x), y(y), z(z) {}
    Matrix(const Matrix<1, 2, T> &src, T z) : x(src.x), y(src.y), z(z) {}
    Matrix(const Matrix<1, 3, T> &src) : x(src.x), y(src.y), z(src.z) {}
    explicit Matrix(const Matrix<1, 4, T> &src) : x(src.x), y(src.y), z(src.z) {}
    ~Matrix() = default;
    const T &operator[](size_t index) const { return vec[index]; }
    T &operator[](size_t index) { return vec[index]; }
};

template<typename T>
struct Matrix<1, 4, T>
{
    union
    {
        T matrix[1][4];
        T vec[4];
        struct
        {
            T x, y, z, w;
        };
        struct
        {
            T row, g, b, a;
        };
    };
    Matrix() : x(0), y(0), z(0), w(0) {}
    explicit Matrix(T val) : x(val), y(val), z(val), w(val) {}
    Matrix(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    Matrix(const Matrix<1, 2, T> &src, T z, T w) : x(src.x), y(src.y), z(z), w(w) {}
    Matrix(const Matrix<1, 3, T> &src, T w) : x(src.x), y(src.y), z(src.z), w(w) {}
    Matrix(const Matrix<1, 4, T> &src) : x(src.x), y(src.y), z(src.z), w(src.w) {}
    ~Matrix() {}
    const T &operator[](size_t index) const { return vec[index]; }
    T &operator[](size_t index) { return vec[index]; }
};

//==================================================================================================
//别名
//==================================================================================================
//向量
template<typename T>
using Vector2 = Matrix<1, 2, T>;
template<typename T>
using Vector3 = Matrix<1, 3, T>;
template<typename T>
using Vector4 = Matrix<1, 4, T>;
//点坐标
template<typename T>
using Point2 = Matrix<1, 2, T>;
template<typename T>
using Point3 = Matrix<1, 3, T>;
template<typename T>
using Point4 = Matrix<1, 4, T>;
//特化别名
using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector4f = Vector4<float>;
using Point2f = Point2<float>;
using Point3f = Point3<float>;
using Point4f = Point4<float>;
using Point2i = Point2<int>;
//==================================================================================================
//向量输出
//==================================================================================================
template<size_t N, typename T>
std::ostream &operator<<(std::ostream &os, const Vector<N, T> &vector)
{
    os << "{";
    for (size_t i = 0; i < N; i++)
    {
        os << vector[i];
        if (i != N - 1)os << ", ";
    }
    os << "}";
    return os;
}
//==================================================================================================
//矩阵相关函数
//==================================================================================================
//取一行
template<size_t ROW, size_t N, typename T>
inline Vector<N, T> GetRow(const Matrix<ROW, N, T> &matrix, size_t row)
{
    Vector<N, T> vector;
    for (size_t col = 0; col < N; col++)
        vector[col] = matrix[row][col];
    return vector;
}
//取一列
template<size_t ROW, size_t N, typename T>
inline Vector<ROW, T> GetCol(const Matrix<ROW, N, T> &matrix, size_t col)
{
    Vector<ROW, T> vector;
    for (size_t row = 0; row < ROW; row++)
        vector[row] = matrix[row][col];
    return vector;
}
//特化四阶矩阵与四维向量乘法
template<typename T>
inline Vector<4, T> operator*(const Matrix<4, 4, T> &matrix, const Vector<4, T> &vector)
{
    Vector<4, T> result;
    for (size_t row = 0; row < 4; row++)
    {
        result[row] = 0;
        for (size_t col = 0; col < 4; col++)
            result[row] += matrix[row][col] * vector[col];
    }
    return result;
}
//特化四阶矩阵与三维矢量乘法
template<typename T>
Vector3<T> operator*(const Matrix<4, 4, T> &lhs, const Vector3<T> &rhs)
{
    Vector3<T> ret;
    for (size_t row = 0; row < 3; row++)
        for (size_t i = 0; i < 3; i++)
            ret[i] += lhs[row][i] * rhs[i];
    return ret;
}
//==================================================================================================
//向量布尔运算
//==================================================================================================
//v1 == v2
template<size_t N, typename T>
bool operator==(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    for (size_t i = 0; i < N; i++)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}
//v1 != v2
template<size_t N, typename T>
bool operator!=(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    return !(lhs == rhs);
}
//==================================================================================================
//向量正负
//==================================================================================================
//+v
template<size_t N, typename T>
Vector<N, T> operator+(const Vector<N, T> &vector)
{
    return vector;
}
//-v
template<size_t N, typename T>
Vector<N, T> operator-(const Vector<N, T> &vector)
{
    Vector<N, T> result;
    for (size_t i = 0; i < N; i++)
        result[i] = -vector[i];
    return result;
}
//==================================================================================================
//向量加法
//==================================================================================================
//v1 + v2
template<size_t N, typename T>
Vector<N, T> operator+(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs[i] + rhs[i];
    return ret;
}
//v1 += v2
template<size_t N, typename T>
Vector<N, T> &operator+=(Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    lhs = lhs + rhs;
    return lhs;
}
//v + normal
template<size_t N, typename T>
Vector<N, T> operator+(const Vector<N, T> &lhs, T rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs[i] + rhs;
    return ret;
}
//normal + v
template<size_t N, typename T>
Vector<N, T> operator+(T lhs, const Vector<N, T> &rhs)
{
    return rhs + lhs;
}
//v += normal
template<size_t N, typename T>
Vector<N, T> &operator+=(Vector<N, T> &lhs, T rhs)
{
    lhs = lhs + rhs;
    return lhs;
}
//==================================================================================================
//向量减法
//==================================================================================================
//v1 - v2
template<size_t N, typename T>
Vector<N, T> operator-(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs[i] - rhs[i];
    return ret;
}
//v1 -= v2
template<size_t N, typename T>
Vector<N, T> &operator-=(Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    lhs = lhs - rhs;
    return lhs;
}
//v - normal
template<size_t N, typename T>
Vector<N, T> operator-(const Vector<N, T> &lhs, T rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs[i] - rhs;
    return ret;
}
//normal - v
template<size_t N, typename T>
Vector<N, T> operator-(T lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs - rhs[i];
    return ret;
}
//v -= normal
template<size_t N, typename T>
Vector<N, T> &operator-=(Vector<N, T> &lhs, T rhs)
{
    lhs = lhs - rhs;
    return lhs;
}
//==================================================================================================
//向量乘法
//==================================================================================================
//v1 * v2
template<size_t N, typename T>
Vector<N, T> operator*(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs[i] * rhs[i];
    return ret;
}
//v1 *= v2
template<size_t N, typename T>
Vector<N, T> &operator*=(Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    lhs = lhs * rhs;
    return lhs;
}
//v * normal
template<size_t N, typename T>
Vector<N, T> operator*(const Vector<N, T> &lhs, T rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs[i] * rhs;
    return ret;
}
//normal * v
template<size_t N, typename T>
Vector<N, T> operator*(T lhs, const Vector<N, T> &rhs)
{
    return rhs * lhs;
}
//v *= normal
template<size_t N, typename T>
Vector<N, T> &operator*=(Vector<N, T> &lhs, T rhs)
{
    lhs = lhs * rhs;
    return lhs;
}
//==================================================================================================
//向量除法
//==================================================================================================
//v1 / v2
template<size_t N, typename T>
Vector<N, T> operator/(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs[i] / rhs[i];
    return ret;
}
//v1 /= v2
template<size_t N, typename T>
Vector<N, T> &operator/=(Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    lhs = lhs / rhs;
    return lhs;
}
//v / normal
template<size_t N, typename T>
Vector<N, T> operator/(const Vector<N, T> &lhs, T rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs[i] / rhs;
    return ret;
}
//normal / v
template<size_t N, typename T>
Vector<N, T> operator/(T lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = lhs / rhs[i];
    return ret;
}
//v /= normal
template<size_t N, typename T>
Vector<N, T> &operator/=(Vector<N, T> &lhs, T rhs)
{
    lhs = lhs / rhs;
    return lhs;
}
//==================================================================================================
//向量运算函数
//==================================================================================================
//向量长度平方
template<size_t N, typename T>
inline T LengthSquare(const Vector<N, T> &vector)
{
    T ret = 0;
    for (size_t i = 0; i < N; i++)
        ret += vector[i] * vector[i];
    return ret;
}
//向量长度
template<size_t N, typename T>
inline T Length(const Vector<N, T> &vector)
{
    return std::sqrt(LengthSquare(vector));
}
//特化float向量长度
template<size_t N>
inline float Length(const Vector<N, float> &vector)
{
    float ret = 0;
    for (size_t i = 0; i < N; i++)
        ret += vector[i] * vector[i];
    return std::sqrtf(LengthSquare(vector));
}
//点距离
template<size_t N, typename T>
inline T Distance(const Vector<N, T> &v1, const Vector<N, T> &v2)
{
    return Length(v1 - v2);
}
//向量归一化
template<size_t N, typename T>
inline Vector<N, T> Normalize(const Vector<N, T> &vector)
{
    return vector / Length(vector);
}
//向量点乘
template<size_t N, typename T>
inline T Dot(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    T ret = 0;
    for (size_t i = 0; i < N; i++)
        ret += lhs[i] * rhs[i];
    return ret;
}
//三维向量/齐次向量叉乘
template<size_t N, typename T>
inline Vector<N, T> Cross(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    ret[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    ret[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    ret[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
    return ret;
}
//二维向量叉乘
template<typename T>
inline T Cross(const Vector<2, T> &lhs, const Vector<2, T> &rhs)
{
    return lhs[0] * rhs[1] - lhs[1] * rhs[0];
}
//==================================================================================================
//向量工具函数
//==================================================================================================
//绝对值
template<size_t N, typename T>
inline Vector<N, T> Abs(const Vector<N, T> &vector)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = std::abs(vector[i]);
    return ret;
}
//MaxComponent
template<size_t N, typename T>
inline T MaxComponent(const Vector<N, T> &vector)
{
    T maxVal = vector[0];
    for (size_t i = 1; i < N; i++)
        if (vector[i] > maxVal)
            maxVal = vector[i];
    return maxVal;
}
//MinComponent
template<size_t N, typename T>
inline T MinComponent(const Vector<N, T> &vector)
{
    T minVal = vector[0];
    for (size_t i = 1; i < N; i++)
        if (vector[i] < minVal)
            minVal = vector[i];
    return minVal;
}
//Max
template<size_t N, typename T>
inline Vector<N, T> Max(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = std::max(lhs[i], rhs[i]);
    return ret;
}
//Min
template<size_t N, typename T>
inline Vector<N, T> Min(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++)
        ret[i] = std::min(lhs[i], rhs[i]);
    return ret;
}
//MaxDimension
template<size_t N, typename T>
inline size_t MaxDimension(const Vector<N, T> &vector)
{
    size_t maxDim = 0;
    for (size_t i = 1; i < N; i++)
        if (vector[i] > vector[maxDim])
            maxDim = i;
    return maxDim;
}
//MinDimension
template<size_t N, typename T>
inline size_t MinDimension(const Vector<N, T> &vector)
{
    size_t minDim = 0;
    for (size_t i = 1; i < N; i++)
        if (vector[i] < vector[minDim])
            minDim = i;
    return minDim;
}
//CoordinateSystem
inline void CoordinateSystem(const Vector3f &v1, Vector3f *v2, Vector3f *v3)
{
    if (std::abs(v1.x) > std::abs(v1.y))
    {
        float invLen = 1.0f / std::sqrt(v1.x * v1.x + v1.z * v1.z);
        *v3 = Vector3f(-v1.z * invLen, 0.0f, v1.x * invLen);
    }
    else
    {
        float invLen = 1.0f / std::sqrt(v1.y * v1.y + v1.z * v1.z);
        *v3 = Vector3f(0.0f, v1.z * invLen, -v1.y * invLen);
    }
    *v2 = Cross(v1, *v3);
}
