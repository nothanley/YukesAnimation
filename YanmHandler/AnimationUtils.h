/* Defines common logic and structures found throughout all serialized motion streams */
#pragma once
#include <windows.h> 
#include <winsock.h>
#include <vector>
#include <istream>

/* Angle conversion constants */
const float URotToDegree = 0.005493247883;

struct Vec4 {
    float w, x, y, z;
};

struct Vec3 {
	float x, y, z;
};

struct Matrix3x3 {
    Vec3 row0;
    Vec3 row1;
    Vec3 row2;
};

struct Matrix4x4 {
    Vec4 row0;
    Vec4 row1;
    Vec4 row2;
    Vec4 row3;
};

struct TranslateKey {
    Vec3 transform;
    uint16_t duration;
};

struct MatrixKey {
    Matrix3x3 transform;
    uint16_t duration;
};

namespace AnimUtils {
    void StreamMatrix3x3(std::istream* fs, Matrix3x3* mat);
};