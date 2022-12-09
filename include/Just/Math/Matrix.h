#pragma once

#include "Just/Common.h"

template<size_t ROW, size_t COL, typename T>
struct Matrix
{
    T matrix[ROW][COL];
    Matrix()
    {
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                matrix[i][j] = 0;
    }
    Matrix(std::initializer_list<std::initializer_list<T>> list)
    {
        size_t i = 0;
        for (auto &row: list)
        {
            size_t j = 0;
            for (auto &col: row)
            {
                matrix[i][j] = col;
                j++;
            }
            i++;
        }
    }
    Matrix(const Matrix<ROW, COL, T> &src)
    {
        for (size_t row = 0; row < ROW; row++)
            for (size_t col = 0; col < COL; col++)
                matrix[row][col] = src.matrix[row][col];
    }
    ~Matrix() = default;
    const T *operator[](size_t index) const { return matrix[index]; }
    T *operator[](size_t index) { return matrix[index]; }
    static Matrix<ROW, COL, T> Identity()
    {
        Matrix<ROW, COL, T> m;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                m.matrix[i][j] = (i == j) ? 1 : 0;
        return m;
    }
};

//==================================================================================================
//别名
//==================================================================================================
template<size_t COL, typename T>
using Vector = Matrix<1, COL, T>;
template<size_t COL, typename T>
using Point = Matrix<1, COL, T>;
template<typename T>
using Matrix3x3 = Matrix<3, 3, T>;
template<typename T>
using Matrix4x4 = Matrix<4, 4, T>;
//特化别名
using Matrix3f = Matrix3x3<float>;
using Matrix4f = Matrix4x4<float>;
//==================================================================================================
//矩阵输出
//==================================================================================================
template<size_t ROW, size_t COL, typename T>
inline std::ostream &operator<<(std::ostream &os, const Matrix<ROW, COL, T> &mat)
{
    for (size_t row = 0; row < ROW; row++)
    {
        os << "{";
        for (size_t col = 0; col < COL; col++)
        {
            os << mat[row][col];
            if (col != COL - 1)
                os << ", ";
        }
        os << "}";
        if (row != ROW - 1)
            os << std::endl;
    }
    return os;
}
//==================================================================================================
//矩阵正负
//==================================================================================================
//+m
template<size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator+(const Matrix<ROW, COL, T> &mat)
{
    return mat;
}
//-m
template<size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator-(const Matrix<ROW, COL, T> &mat)
{
    Matrix<ROW, COL, T> result;
    for (size_t row = 0; row < ROW; row++)
        for (size_t col = 0; col < COL; col++)
            result[row][col] = -mat[row][col];
    return result;
}
//==================================================================================================
//矩阵乘法
//==================================================================================================
//m1 * m2
template<size_t ROW, size_t COM, size_t COL, typename T>
Matrix<ROW, COL, T> operator*(const Matrix<ROW, COM, T> &mat1, const Matrix<COM, COL, T> &mat2)
{
    Matrix<ROW, COL, T> ret;
    for (size_t row = 0; row < ROW; row++)
        for (size_t col = 0; col < COL; col++)
            for (size_t i = 0; i < COM; i++)
                ret[row][col] += mat1[row][i] * mat2[i][col];
    return ret;
}
//m * n
template<size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator*(const Matrix<ROW, COL, T> &mat, T val)
{
    Matrix<ROW, COL, T> ret;
    for (size_t row = 0; row < ROW; row++)
        for (size_t col = 0; col < COL; col++)
            ret[row][col] = mat[row][col] * val;
    return ret;
}
//n * m
template<size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator*(T val, const Matrix<ROW, COL, T> &mat2)
{
    return mat2 * val;
}
//==================================================================================================
//矩阵除法
//==================================================================================================
//matrix / n
template<size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator/(const Matrix<ROW, COL, T> &mat, T val)
{
    return mat * (1.0f / val);
}
//n / matrix
template<size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> operator/(T val, const Matrix<ROW, COL, T> &mat)
{
    return mat * (1.0f / val);
}
//matrix /= n
template<size_t ROW, size_t COL, typename T>
Matrix<ROW, COL, T> &operator/=(Matrix<ROW, COL, T> &mat, T val)
{
    mat *= 1.0f / val;
    return mat;
}
//==================================================================================================
//矩阵运算函数
//==================================================================================================
//矩阵转置
template<size_t ROW, size_t COL, typename T>
inline Matrix<COL, ROW, T> Transpose(const Matrix<ROW, COL, T> &mat)
{
    Matrix<COL, ROW, T> ret;
    for (size_t row = 0; row < ROW; row++)
        for (size_t col = 0; col < COL; col++)
            ret[col][row] = mat[row][col];
    return ret;
}
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW - 1, COL - 1, T> Minor(const Matrix<ROW, COL, T> &mat, size_t i, size_t j)
{
    Matrix<ROW - 1, COL - 1, T> minor;
    for (size_t row = 0; row < ROW - 1; row++)
        for (size_t col = 0; col < COL - 1; col++)
            minor.matrix[row][col] = mat[row < i ? row : row + 1][col < j ? col : col + 1];
    return minor;
}
//一阶行列式
template<typename T>
inline T Det(const Matrix<1, 1, T> &mat)
{
    return mat[0][0];
}
//二阶行列式
template<typename T>
inline T Det(const Matrix<2, 2, T> &mat)
{
    return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}
//多阶行列式
template<size_t N, typename T>
inline T Det(const Matrix<N, N, T> &mat)
{
    T ret = 0;
    for (size_t i = 0; i < N; i++)
        ret += mat[0][i] * Cofactor(mat, 0, i);
    return ret;
}
//一阶余子式
template<typename T>
inline T Cofactor(const Matrix<1, 1, T> &mat, size_t row, size_t col)
{
    return 0;
}
//多阶余子式
template<size_t N, typename T>
inline T Cofactor(const Matrix<N, N, T> &mat, size_t row, size_t col)
{
    return Det(Minor(mat, row, col)) * (((row + col) % 2) ? -1 : 1);
}
//伴随矩阵
template<size_t N, typename T>
inline Matrix<N, N, T> Adjoint(const Matrix<N, N, T> &mat)
{
    Matrix<N, N, T> ret;
    for (size_t row = 0; row < N; row++)
        for (size_t col = 0; col < N; col++)
            ret[col][row] = Cofactor(mat, row, col);
    return ret;
}
//逆矩阵
template<size_t N, typename T>
inline Matrix<N, N, T> Inverse(const Matrix<N, N, T> &mat)
{
    return Adjoint(mat) / Det(mat);
}

