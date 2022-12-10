#include "Just/Common.h"
#include "Just/Math/Matrix.h"
#include "Just/Math/Vector.h"

void TestVector()
{
    Vector<3, float> v1{1, 2, 1};
    Vector<3, float> v2{2, 1, 2};
    //向量布尔运算
    std::cout << "====================================================" << std::endl;
    std::cout << "{1, 2, 1} == {2, 1, 2} -> " << (v1 == v2) << std::endl;
    std::cout << "{1, 2, 1} != {2, 1, 2} -> " << (v1 != v2) << std::endl;
    //向量加法
    std::cout << "====================================================" << std::endl;
    std::cout << "+{1, 2, 1} = " << v1 << std::endl;
    std::cout << "{1, 2, 1} + {2, 1, 2} = " << (v1 + v2) << std::endl;
    std::cout << "{2, 1, 2} += {1, 2, 1} -> " << (v2 += v1) << std::endl;
    v2 = Vector<3, float>{2, 1, 2};
    //向量减法
    std::cout << "====================================================" << std::endl;
    std::cout << "-{1, 2, 1} = " << -v1 << std::endl;
    std::cout << "{1, 2, 1} - {2, 1, 2} = " << (v1 - v2) << std::endl;
    std::cout << "{2, 1, 2} -= {1, 2, 1} -> " << (v2 -= v1) << std::endl;
    v2 = Vector<3, float>{2, 1, 2};
    //向量数加
    std::cout << "====================================================" << std::endl;
    std::cout << "{2, 1, 2} + 1 = " << (v2 + 1.0f) << std::endl;
    std::cout << "1 + {2, 1, 2} = " << (1.0f + v2) << std::endl;
    std::cout << "{2, 1, 2} += 1.0f -> " << (v2 += 1.0f) << std::endl;
    v2 = Vector<3, float>{2, 1, 2};
    //向量数减
    std::cout << "====================================================" << std::endl;
    std::cout << "{2, 1, 2} - 1 = " << (v2 - 1.0f) << std::endl;
    std::cout << "1 - {2, 1, 2} = " << (1.0f - v2) << std::endl;
    std::cout << "{2, 1, 2} -= 1.0f -> " << (v2 -= 1.0f) << std::endl;
    v2 = Vector<3, float>{2, 1, 2};
    //向量乘法
    std::cout << "====================================================" << std::endl;
    std::cout << "{1, 2, 1} * {2, 1, 2} = " << (v1 * v2) << std::endl;
    std::cout << "{2, 1, 2} *= {1, 2, 1} -> " << (v2 *= v1) << std::endl;
    v2 = Vector<3, float>{2, 1, 2};
    //向量数乘
    std::cout << "====================================================" << std::endl;
    std::cout << "{2, 1, 2} * 2.0f = " << (v2 * 2.0f) << std::endl;
    std::cout << "2.0f * {2, 1, 2} = " << (2.0f * v2) << std::endl;
    std::cout << "{2, 1, 2} *= 2.0f -> " << (v2 *= 2.0f) << std::endl;
    v2 = Vector<3, float>{2, 1, 2};
    //向量除法
    std::cout << "====================================================" << std::endl;
    std::cout << "{1, 2, 1} / {2, 1, 2} = " << (v1 / v2) << std::endl;
    std::cout << "{2, 1, 2} /= {1, 2, 1} -> " << (v2 /= v1) << std::endl;
    v2 = Vector<3, float>{2, 1, 2};
    //向量数除
    std::cout << "====================================================" << std::endl;
    std::cout << "{2, 1, 2} / 2.0f = " << (v2 / 2.0f) << std::endl;
    std::cout << "2.0f / {2, 1, 2} = " << (2.0f / v2) << std::endl;
    std::cout << "{2, 1, 2} /= 2.0f -> " << (v2 /= 2.0f) << std::endl;
    v2 = Vector<3, float>{2, 1, 2};
    //向量点乘
    std::cout << "====================================================" << std::endl;
    std::cout << "{1, 2, 1} dot {2, 1, 2} = " << Dot(v1, v2) << std::endl;
    std::cout << "{1, 2, 1, 0} dot {2, 1, 2, 0} = " << Dot(Vector4f{v1, 0}, Vector4f{v2, 0}) << std::endl;
    //向量叉乘
    std::cout << "====================================================" << std::endl;
    std::cout << "{1, 1} cross {2, 2} = " << Cross(Vector2f{v1}, Vector2f{v2}) << std::endl;
    std::cout << "{1, 2, 1} cross {2, 1, 2} = " << Cross(v1, v2) << std::endl;
    std::cout << "{1, 1, 1, 0} cross {2, 2, 2, 0} = " << Cross(Vector4f{v1, 0}, Vector4f{v2, 0}) << std::endl;
    //向量长度
    std::cout << "====================================================" << std::endl;
    std::cout << "{0, 3, 4} length = " << Length(Vector3f{0, 3, 4}) << std::endl;
    std::cout << "{0, 3, 4} normalize = " << Normalize(Vector3f{0, 3, 4}) << std::endl;
    std::cout << "{0, 3, 4} length square = " << LengthSquare(Vector3f{0, 3, 4}) << std::endl;
    std::cout << "{-1, 2, -3} abs = " << Abs(Vector3f{-1, 2, -3}) << std::endl;
    std::cout << "{-1, 2, -3} max component= " << MaxComponent(Vector3f{-1, 2, -3}) << std::endl;
    std::cout << "{-1, 2, -3} min component= " << MinComponent(Vector3f{-1, 2, -3}) << std::endl;
    std::cout << "{-1, 2, -3} max {-2, 3, -4} = " << Max(Vector3f{-1, 2, -3}, Vector3f{-2, 3, -4}) << std::endl;
    std::cout << "{-1, 2, -3} min {-2, 3, -4} = " << Min(Vector3f{-1, 2, -3}, Vector3f{-2, 3, -4}) << std::endl;
}
void TestMatrix()
{
    Matrix<2, 3, float> m1{
            {1, 3, 2},
            {4, 0, 1}
    };
    Matrix<3, 2, float> m2{
            {1, 3},
            {0, 1},
            {5, 2}
    };
    //矩阵正负
    std::cout << "====================================================" << std::endl;
    std::cout << "+m1 = " << std::endl << +m1 << std::endl;
    std::cout << "-m2 = " << std::endl << -m2 << std::endl;
    //矩阵数乘
    std::cout << "====================================================" << std::endl;
    std::cout << "m1 * 2.0f = " << std::endl << (m1 * 2.0f) << std::endl;
    std::cout << "2.0f * m2 = " << std::endl << (2.0f * m2) << std::endl;
    //矩阵乘法
    std::cout << "====================================================" << std::endl;
    std::cout << "m1 * m2 = " << std::endl << (m1 * m2) << std::endl;
    Matrix3f m3{
            {1, 2,  -1},
            {3, 4,  -2},
            {5, -4, 1}
    };
    //矩阵运算
    std::cout << "====================================================" << std::endl;
    std::cout << "m3 = " << std::endl << m3 << std::endl;
    std::cout << "m1 minor 0, 0 = " << std::endl << Minor(m3, 0, 0) << std::endl;
    std::cout << "m1 cofactor 0, 0 = " << Cofactor(m3, 0, 0) << std::endl;
    std::cout << "m1 adjoint = " << std::endl << Adjoint(m3) << std::endl;
    std::cout << "m1 det = " << Det(m3) << std::endl;
    std::cout << "m3 inverse = " << std::endl << Inverse(m3) << std::endl;
}
int main()
{
    TestVector();
    TestMatrix();
}