/* Defines common logic and structures found throughout all serialized motion streams */
#pragma once
#include <windows.h> 
#include <winsock.h>
#include <vector>
#include <istream>

/* Angle conversion constants */
const float U16RotToDegree = 0.005493247883;
const float DegConst  = 1.417322834;
const float DegUnk = 0.000681769;
const float SByteToDeg = 1.417322834;
const float S8RotToDegree = 2.834645669;

struct Vec4 {
    float w, x, y, z;
};

struct Vec3 {
	float x, y, z;
};

inline Vec3& operator*(Vec3& vec_a, Vec3& vec_b) {
    Vec3 result = { vec_a.x * vec_b.x,
                    vec_a.y * vec_b.y,
                    vec_a.z * vec_b.z };
    return result;
}

inline Vec3& operator-(Vec3& vec_a, Vec3& vec_b) {
    Vec3 result = { vec_a.x - vec_b.x,
                    vec_a.y - vec_b.y,
                    vec_a.z - vec_b.z };
    return result;
}

inline Vec3& operator+(Vec3& vec_a, Vec3& vec_b) {
    Vec3 result = { vec_a.x + vec_b.x,
                    vec_a.y + vec_b.y,
                    vec_a.z + vec_b.z };
    return result;
}

struct Matrix3x3 {
    Vec3 row0;
    Vec3 row1;
    Vec3 row2;
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

struct TranslateKey {
    Vec3 transform;
    uint16_t duration;
};

struct MatrixKey {
    Matrix3x3 transform;
    uint16_t duration;
};

struct IKKey {
    Vec4 transform;
    uint16_t duration;
};

namespace AnimUtils {

    void StreamMatrix3x3(std::istream* fs, Matrix3x3* mat);

    Matrix3x4 GetIKOriginMatrix(std::istream* fs);

    Vec4 UnpackMatrixVector(Matrix3x4 mat);

    Vec4 Extract32bitOrigin(std::istream* fs);

    Vec4 Extract24bitOrigin(std::istream* fs);

    Vec4 Extract16bitOrigin(std::istream* fs);

    float ExtractBits(uint16_t ubits);

    Vec4 ShiftIKMatrix(std::istream* fs, Matrix3x4 mat, uint8_t encodeSize);

    void GetAnimOrigin(std::istream* fs, Vec4* origin);

    void DecodeTransStream16S(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* translations);

    void DecodeRotationStream16S(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations);

    void DecodeIKStream(std::istream* fs, uint16_t* numSegments, uint8_t* ikType, Matrix3x4* origin, std::vector<IKKey>* ikTransforms);

    void ReadRotationMatrices(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations);
    
    void DecodeEulerStreamS16(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection);

    void DecodeDeltaStreamS16(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection);

    void DecodeEulerStreamS8(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection);

    void DebugDecodeEulerStreamS8(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection);

    void DecodeRotationStream8S(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations);

    void ApplyVectorDeltas(std::vector<TranslateKey>* vector, Vec4* delta);
};
