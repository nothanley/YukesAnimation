#pragma once
#include <windows.h> 
#include <cmath>

struct Vec4 {
    float w, x, y, z;
};

struct Matrix3x4 {
    Vec4 row0;
    Vec4 row1;
    Vec4 row2;
};

struct Matrix4x4 {
    Vec4 row0;
    Vec4 row1;
    Vec4 row2;
    Vec4 row3;
};

static Vec4 sqrt(const Vec4& vec_a) {
    Vec4 result = { sqrtf(vec_a.w),
                    sqrtf(vec_a.x),
                    sqrtf(vec_a.y),
                    sqrtf(vec_a.z) };

    if (std::isnan(sqrtf(vec_a.w)))
        result = Vec4{ 0,0,0,0 };

    return result;
}

static bool isNan(const Vec4& vec_a) {

    if (std::isnan(vec_a.x)) { return true; }
    if (std::isnan(vec_a.y)) { return true; }
    if (std::isnan(vec_a.z)) { return true; }
    if (std::isnan(vec_a.w)) { return true; }

    return false;
}

static bool isInf(const Vec4& vec_a) {
    
    if (std::isinf(vec_a.x)) { return true; }
    if (std::isinf(vec_a.y)) { return true; }
    if (std::isinf(vec_a.z)) { return true; }
    if (std::isinf(vec_a.w)) { return true; }

    return false;
}

static Vec4 operator*(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w * vec_b.w,
                    vec_a.x * vec_b.x,
                    vec_a.y * vec_b.y,
                    vec_a.z * vec_b.z };
    return result;
}

static Vec4 operator/(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w / vec_b.w,
                    vec_a.x / vec_b.x,
                    vec_a.y / vec_b.y,
                    vec_a.z / vec_b.z };
    return result;
}


static Vec4 operator+(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w + vec_b.w,
                    vec_a.x + vec_b.x,
                    vec_a.y + vec_b.y,
                    vec_a.z + vec_b.z };
    return result;
}

static Vec4 operator-(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w - vec_b.w,
                    vec_a.x - vec_b.x,
                    vec_a.y - vec_b.y,
                    vec_a.z - vec_b.z, };
    return result;
}

static Vec4 operator-(const Vec4& vec_a, const float& sub) {
    Vec4 result = { vec_a.w - sub,
                    vec_a.x - sub,
                    vec_a.y - sub,
                    vec_a.z - sub };
    return result;
}

static Vec4 operator-(const float& sub, const Vec4& vec_a) {
    Vec4 result = { sub - vec_a.w,
                    sub - vec_a.x,
                    sub - vec_a.y,
                    sub - vec_a.z };
    return result;
}

static Vec4 operator+(const float& sub, const Vec4& vec_a) {
    Vec4 result = { sub + vec_a.w,
                    sub + vec_a.x,
                    sub + vec_a.y,
                    sub + vec_a.z };
    return result;
}

static Vec4 operator*(const Vec4& vec_a, const float& b) {
    Vec4 result = { vec_a.w * b,
                    vec_a.x * b,
                    vec_a.y * b,
                    vec_a.z * b };
    return result;
}

static Vec4 operator+(const Vec4& vec_a, const float& b) {
    Vec4 result = { vec_a.w + b,
                    vec_a.x + b,
                    vec_a.y + b,
                    vec_a.z + b };
    return result;
}

static Vec4 operator/(const float& sub, const Vec4& vec_a) {
    Vec4 result = { sub / vec_a.w,
                    sub / vec_a.x,
                    sub / vec_a.y,
                    sub / vec_a.z };
    return result;
}

static Vec4 operator/(const Vec4& vec_a, const float& sub) {
    Vec4 result = { vec_a.w / sub,
                    vec_a.x / sub,
                    vec_a.y / sub,
                    vec_a.z / sub };
    return result;
}


static bool operator==(const Vec4& vec_a, const Vec4& vec_b) {

    if (vec_a.x != vec_b.x) { return false; }
    if (vec_a.y != vec_b.y) { return false; }
    if (vec_a.z != vec_b.z) { return false; }
    if (vec_a.w != vec_b.w) { return false; }

    return true;
}


static double round_to(double value, double precision = 1.0)
{
    return std::round(value / precision) * precision;
}

inline float NormalizeVector(const float& input) {

    float value = (input > 1) ? (1.0 / input) : input;

    float square = value * value;
    float result = square * ((square * 0.00286623) + -0.0161657);
    result = square * ((square * (result + 0.0429096)) + -0.0752896);
    result = square * ((square * (result + 0.106563)) + -0.142089);
    result = square * ((square * (result + 0.199936)) + -0.333331);

    result = (result + 1.0) * value;
    if (input > 1) { result = (1.5708 - result); }

    return result;
}

inline Vec4 InterpolateVector(Vec4 interpVec) {
    Vec4 square = interpVec * interpVec;
    Vec4 result = (square * -2.38899e-008);

    result = result + 2.75256e-006;
    result = result * square;

    result = result + -0.000198409;
    result = result * square;

    result = result + 0.00833333;
    result = result * square;

    result = result + -0.166667;
    result = result * square;
    result = 1.0 + result;

    result = interpVec * result;
    return result;
}

