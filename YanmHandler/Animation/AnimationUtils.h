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


inline Vec4 sqrt(const Vec4& vec_a) {
    Vec4 result = { sqrtf(vec_a.w),
                    sqrtf(vec_a.x),
                    sqrtf(vec_a.y),
                    sqrtf(vec_a.z) };
    return result;
}

inline Vec4 operator*(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w * vec_b.w,
                    vec_a.x * vec_b.x,
                    vec_a.y * vec_b.y,
                    vec_a.z * vec_b.z };
    return result;
}

inline Vec4 operator/(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w / vec_b.w,
                    vec_a.x / vec_b.x,
                    vec_a.y / vec_b.y,
                    vec_a.z / vec_b.z };
    return result;
}


inline Vec4 operator+(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w + vec_b.w,
                    vec_a.x + vec_b.x,
                    vec_a.y + vec_b.y,
                    vec_a.z + vec_b.z };
    return result;
}

inline Vec4 operator-(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w - vec_b.w,
                    vec_a.x - vec_b.x,
                    vec_a.y - vec_b.y,
                    vec_a.z - vec_b.z,};
    return result;
}

inline Vec4 operator-(const Vec4& vec_a, const float& sub ) {
    Vec4 result = { vec_a.w - sub,
                    vec_a.x - sub,
                    vec_a.y - sub,
                    vec_a.z - sub };
    return result;
}

inline Vec4 operator-(const float& sub,const Vec4& vec_a) {
    Vec4 result = { sub - vec_a.w,
                    sub - vec_a.x,
                    sub - vec_a.y,
                    sub - vec_a.z };
    return result;
}

inline Vec4 operator*(const Vec4& vec_a, const float& b) {
    Vec4 result = { vec_a.w * b,
                    vec_a.x * b,
                    vec_a.y * b,
                    vec_a.z * b };
    return result;
}

inline Vec4 operator+(const Vec4& vec_a, const float& b) {
    Vec4 result = { vec_a.w + b,
                    vec_a.x + b,
                    vec_a.y + b,
                    vec_a.z + b };
    return result;
}

inline Vec4 operator/(const float& sub,const Vec4& vec_a) {
    Vec4 result = { sub / vec_a.w,
                    sub / vec_a.x,
                    sub / vec_a.y,
                    sub / vec_a.z };
    return result;
}

inline Vec4 operator/(const Vec4& vec_a, const float& sub) {
    Vec4 result = { vec_a.w / sub,
                    vec_a.x / sub,
                    vec_a.y / sub,
                    vec_a.z / sub };
    return result;
}












inline Vec3 operator*(const Vec3& vec_a, const Vec3& vec_b) {
    Vec3 result = { vec_a.x * vec_b.x,
                    vec_a.y * vec_b.y,
                    vec_a.z * vec_b.z };
    return result;
}


inline Vec3 operator-(const Vec3& vec_a, const Vec3& vec_b) {
    Vec3 result = { vec_a.x - vec_b.x,
                    vec_a.y - vec_b.y,
                    vec_a.z - vec_b.z };
    return result;
}

inline Vec3 operator+(const Vec3& vec_a, const Vec3& vec_b) {
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

    void InterpretU8Byte(Vec3* vector);

    void DebugDecodeEulerStreamS8(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection);

    void Get8bSignedByteArray(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* rotations);

    void DecodeRotationStream8S(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations);

    void ApplyVectorDeltas(std::vector<TranslateKey>* vector, Vec4* delta);

    template <typename KeyType>
    static void GetVectorRuntime(const std::vector<KeyType>& keys, unsigned int* runtime) {
        if (keys.size() == 0) { return; }
        *runtime = 0;
        for (const auto& key : keys)
            *runtime += key.duration;
    }


};
