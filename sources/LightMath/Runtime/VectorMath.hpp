#pragma once
#include <algorithm>
#include <numbers>
#include <cmath>

#include "Vector.hpp"

template <typename Type>
    requires std::is_arithmetic_v<Type>
constexpr Type max(const Type a, const Type b) { return std::max(a, b); }
template <typename Type>
    requires std::is_arithmetic_v<Type>
constexpr Type min(const Type a, const Type b) { return std::min(a, b); }
template <typename Type>
    requires std::is_arithmetic_v<Type>
constexpr Type rsqrt(const Type a) { return 1 / static_cast<Type>(sqrt(a)); }

/**
 * 弧度转角度
 * @param radian 
 * @return 
 */
template <typename Type>
    requires std::is_arithmetic_v<Type>
constexpr Type degrees(const Type radian)
{
    constexpr Type Rad2Deg = Type(180) / std::numbers::pi_v<Type>;
    return radian * Rad2Deg;
}
/**
 * 角度转弧度
 * @param degree 
 * @return 
 */
template <typename Type>
    requires std::is_arithmetic_v<Type>
constexpr Type radians(const Type degree)
{
    constexpr Type Deg2Rad = std::numbers::pi_v<Type> / Type(180);
    return degree * Deg2Rad;
}

//模板没法触发自动类型转换，因此只能利用宏来大批量定义函数

#define MakeFunction_SymbolV1V(Symbol,Type,Number)\
constexpr vector<Type, Number>& operator##Symbol##(vector<Type, Number>& a)\
{\
for (int i = 0; i < (Number); i++)\
Symbol##a[i];\
return a;\
}

#define MakeFunction_SymbolV1VSuf(Symbol,Type,Number)\
constexpr vector<Type, Number> operator##Symbol##(vector<Type, Number>& a,int)\
{\
vector<Type, Number> temp = a;\
for (int i = 0; i < (Number); i++)\
Symbol##a[i];\
return temp;\
}

#define MakeFunction_SymbolVV1V(Symbol,Type,Number)\
constexpr vector<Type, Number>& operator##Symbol##(vector<Type, Number>& a, const vector<Type, Number>& b)\
{\
for (int i = 0; i < (Number); i++)\
a[i] Symbol b[i];\
return a;\
}

#define MakeFunction_SymbolVV2V(Symbol,Type,Number)\
constexpr vector<Type, Number> operator##Symbol##(const vector<Type, Number>& a, const vector<Type, Number>& b)\
{\
    vector<Type, Number> result;\
    for (int i = 0; i < (Number); i++)\
        result[i] = a[i] Symbol b[i];\
    return result;\
}

#define MakeFunction_FunctionV2V(Function,Type,Number)\
constexpr vector<Type, Number> Function##(const vector<Type, Number>& a)\
{\
vector<Type, Number> result;\
for (int i = 0; i < (Number); i++)\
result[i] = Function(a[i]);\
return result;\
}

#define MakeFunction_FunctionVV2V(Function,Type,Number)\
constexpr vector<Type, Number> Function##(const vector<Type, Number>& a, const vector<Type, Number>& b)\
{\
vector<Type, Number> result;\
for (int i = 0; i < (Number); i++)\
result[i] = std::Function(a[i],b[i]);\
return result;\
}

#define MakeFunction_FunctionVVV2V(Function,Type,Number)\
constexpr vector<Type, Number> Function##(const vector<Type, Number>& a, const vector<Type, Number>& b, const vector<Type, Number>& c)\
{\
vector<Type, Number> result;\
for (int i = 0; i < (Number); i++)\
result[i] = std::Function(a[i], b[i], c[i]);\
return result;\
}

#define MakeFunction_Equal(Type,Number)\
constexpr vector<bool, Number> operator==(const vector<Type, Number>& a, const vector<Type, Number>& b)\
{\
vector<bool, Number> result;\
for (int i = 0; i < (Number); i++)\
    result[i] = abs(a[i] - b[i]) <= std::numeric_limits<Type>::epsilon();\
return result;\
}\
constexpr vector<bool, Number> operator!=(const vector<Type, Number>& a, const vector<Type, Number>& b)\
{\
vector<bool, Number> result;\
for (int i = 0; i < (Number); i++)\
result[i] = abs(a[i] - b[i]) > std::numeric_limits<Type>::epsilon();\
return result;\
}

#define MakeFunction_All(Type,Number)\
constexpr bool all(const vector<Type, Number>& a)\
{\
    for (int i = 0; i < (Number); i++)\
        if (a[i] == 0)\
            return false;\
    return true;\
}

#define MakeFunction_Dot(Type,Number)\
constexpr Type dot(const vector<Type, Number>& a, const vector<Type, Number>& b)\
{\
Type result = 0;\
for (int i = 0; i < (Number); i++)\
result += a[i] * b[i];\
return result;\
}

#define MakeFunction_Length(Type,Number)\
constexpr Type lengthsq(const vector<Type, Number>& a)\
{\
return dot(a, a);\
}\
inline Type length(const vector<Type, Number>& a)\
{\
return sqrt(lengthsq(a));\
}

#define MakeFunction_Normalize(Type,Number)\
constexpr vector<Type, Number> normalize(const vector<Type, Number>& a)\
{\
return a / length(a);\
}

/**
 * 线性插值
 * @tparam Type 
 * @tparam Number 
 * @param origin 起点
 * @param destination 终点
 * @param rate 插值比率
 * @return 
 */
#define MakeFunction_Lerp(Type,Number)\
constexpr vector<Type, Number> lerp(const vector<Type, Number>& origin, const vector<Type, Number>& destination, const Type rate)\
{\
    return origin + rate * (destination - origin);\
}

/**
 * 求两个向量的夹角角度
 * @tparam Type 
 * @tparam Number 
 * @param a 
 * @param b 
 * @return 
 */
#define MakeFunction_Angle(Type,Number)\
constexpr Type angle(const vector<Type, Number>& a, const vector<Type, Number>& b)\
{\
    return degrees(acos(dot(a, b) / sqrt(lengthsq(a) * lengthsq(b))));\
}

/**
 * 向量投影
 * @tparam Type 
 * @tparam Number 
 * @param v 被投影向量
 * @param n 投影到的单位法线
 * @return 
 */
#define MakeFunction_Project(Type,Number)\
constexpr vector<Type, Number> project(const vector<Type, Number>& v, const vector<Type, Number>& n)\
{\
    return n * dot(v, n);\
}

#define MakeFunction_ToString(Type,Number)\
std::string to_string(vector<Type, Number> value)\
{\
    std::string str = "(";\
    for (int i = 0; i < (Number); i++)\
        str += std::format("{:f},", value[i]);\
    str.pop_back();\
    return str + ")";\
}

#define MakeFunction_Vector(Type,Number)\
MakeFunction_SymbolV1V(++, Type, Number);\
MakeFunction_SymbolV1V(--, Type, Number);\
MakeFunction_SymbolV1VSuf(++, Type, Number);\
MakeFunction_SymbolV1VSuf(--, Type, Number);\
MakeFunction_SymbolVV1V(+=, Type, Number);\
MakeFunction_SymbolVV1V(-=, Type, Number);\
MakeFunction_SymbolVV1V(*=, Type, Number);\
MakeFunction_SymbolVV1V(/=, Type, Number);\
MakeFunction_SymbolVV2V(+, Type, Number);\
MakeFunction_SymbolVV2V(-, Type, Number);\
MakeFunction_SymbolVV2V(*, Type, Number);\
MakeFunction_SymbolVV2V(/, Type, Number);\
MakeFunction_FunctionV2V(cos, Type, Number);\
MakeFunction_FunctionV2V(acos, Type, Number);\
MakeFunction_FunctionV2V(sin, Type, Number);\
MakeFunction_FunctionV2V(asin, Type, Number);\
MakeFunction_FunctionV2V(tan, Type, Number);\
MakeFunction_FunctionV2V(atan, Type, Number);\
MakeFunction_FunctionV2V(degrees, Type, Number);\
MakeFunction_FunctionV2V(radians, Type, Number);\
MakeFunction_FunctionV2V(sqrt, Type, Number);\
MakeFunction_FunctionV2V(rsqrt, Type, Number);\
MakeFunction_FunctionV2V(abs, Type, Number);\
MakeFunction_FunctionV2V(round, Type, Number);\
MakeFunction_FunctionV2V(trunc, Type, Number);\
MakeFunction_FunctionV2V(ceil, Type, Number);\
MakeFunction_FunctionV2V(floor, Type, Number);\
MakeFunction_FunctionVV2V(fmod, Type, Number);\
MakeFunction_FunctionVV2V(pow, Type, Number);\
MakeFunction_FunctionVV2V(max, Type, Number);\
MakeFunction_FunctionVV2V(min, Type, Number);\
MakeFunction_FunctionVVV2V(clamp, Type, Number);\
MakeFunction_Equal(Type, Number);\
MakeFunction_All(Type, Number);\
MakeFunction_Dot(Type, Number);\
MakeFunction_Length(Type, Number);\
MakeFunction_Normalize(Type, Number);\
MakeFunction_Lerp(Type, Number);\
MakeFunction_Angle(Type, Number);\
MakeFunction_Project(Type, Number);\
MakeFunction_ToString(Type, Number);


/**
 * 三维向量叉乘
 * @param Type 
 */
#define MakeFunction_Cross3(Type)\
constexpr vector<Type, 3> cross(const vector<Type, 3>& a, const vector<Type, 3>& b)\
{\
return {\
a.y * b.z - a.z * b.y,\
-(a.x * b.z - a.z * b.x),\
a.x * b.y - a.y * b.x\
};\
}

/**
 * 三维向量绕轴旋转
 * @tparam Type 
 * @param v 被旋转的向量
 * @param n 作为旋转轴的单位向量
 * @param angle 旋转的角度
 * @return 
 */
#define MakeFunction_Rotate3(Type)\
constexpr vector<Type, 3> rotate(const vector<Type, 3>& v, const vector<Type, 3>& n, const Type angle)\
{\
Type rad = radians(angle);\
vector<Type, 3> parallel = project(v, n);\
vector<Type, 3> perpendicular = v - parallel;\
return perpendicular * cos(rad) + cross(n, perpendicular) * sin(rad) + parallel;\
}

#define MakeFunction_Vector3Plus(Type)\
MakeFunction_Cross3(Type)\
MakeFunction_Rotate3(Type)


MakeFunction_Vector(float, 2)
MakeFunction_Vector(float, 3)
MakeFunction_Vector(float, 4)
MakeFunction_Vector3Plus(float)
using float2 = vector<float, 2>;
using float3 = vector<float, 3>;
using float4 = vector<float, 4>;

MakeFunction_All(bool, 2)
MakeFunction_All(bool, 3)
MakeFunction_All(bool, 4)
using bool2 = vector<bool, 2>;
using bool3 = vector<bool, 3>;
using bool4 = vector<bool, 4>;
