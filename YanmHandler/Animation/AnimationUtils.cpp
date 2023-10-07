#include "../DataIO/BinaryIO.h"
#include "AnimationUtils.h"
using namespace BinaryIO;

void AnimUtils::StreamMatrix3x3(std::istream* fs, Matrix3x3* mat){
	// float URotToDegree = 1.0; pretransform
	mat->row0 = Vec3{ URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };
	mat->row1 = Vec3{ URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };
	mat->row2 = Vec3{ URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };
}

Vec4 AnimUtils::UnpackMatrixVector(Matrix3x4 mat) {
    Vec4 out;
    out.z = (mat.row0.z * mat.row1.z) + mat.row2.z;
    out.y = (mat.row0.y * mat.row1.y) + mat.row2.y;
    out.x = (mat.row0.x * mat.row1.x) + mat.row2.x;
    out.w = (mat.row0.w * mat.row1.w) + mat.row2.w;
    return out;
}

Vec4 AnimUtils::ExtractOrigin(std::istream* fs) {
    uint32_t var0 = ReadByte(*fs) << 0x8;
    uint32_t var1 = ReadByte(*fs) << 0x8;
    uint32_t var2 = ReadByte(*fs) << 0x8;
    uint32_t var3 = ReadByte(*fs) << 0x8;

    var0 |= ReadByte(*fs);
    var1 |= ReadByte(*fs);
    var2 |= ReadByte(*fs);
    var3 |= ReadByte(*fs);
    return Vec4{ (float)var3, (float)var2, (float)var1, (float)var0 };
}

float AnimUtils::ExtractBits(uint16_t ubits) {
    uint64_t value_0;
    uint64_t value_1;
    uint64_t value_2;

    value_0 = (ubits & 0x3FF) << 0xD;
    value_1 = (((ubits >> 0xA) & 0x1F) + 0x70) << 0x17;
    value_2 = (ubits << 0x10) & 0x80000000;
    int32_t result = value_0 | (value_1 | value_2);;

    return *reinterpret_cast<float*>(&result);
}

Vec4 AnimUtils::ShiftIKMatrix(std::istream* fs, Matrix3x4 mat) {
    mat.row0 = ExtractOrigin(fs);
    return UnpackMatrixVector(mat);
}

void AnimUtils::DecodeTransStream16S(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* translations){
    for (int k = 0; k < *numSegments; k++) {
        Vec3 transform = { ReadShortBE(*fs),
            ReadShortBE(*fs), ReadShortBE(*fs) };

        uint16_t numKeys = ReadUShortBE(*fs);
        translations->push_back(TranslateKey{ transform, numKeys });
    }
}

void AnimUtils::DecodeRotationStream16S(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations) {
    for (int k = 0; k < *numSegments; k++) {
        Matrix3x3 mat;
        mat.row0 = { URotToDegree * ReadShortBE(*fs),
            URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };

        uint16_t numKeys = ReadUShortBE(*fs);
        rotations->push_back(MatrixKey{ mat, numKeys });
    }
}

Matrix3x4 AnimUtils::GetIKOriginMatrix(std::istream* fs) {
    Matrix3x4 mat;
    mat.row2.z = ExtractBits(ReadUShortBE(*fs));
    mat.row2.y = ExtractBits(ReadUShortBE(*fs));
    mat.row2.x = ExtractBits(ReadUShortBE(*fs));
    mat.row2.w = ExtractBits(ReadUShortBE(*fs));
    mat.row1.z = ExtractBits(ReadUShortBE(*fs));
    mat.row1.y = ExtractBits(ReadUShortBE(*fs));
    mat.row1.x = ExtractBits(ReadUShortBE(*fs));
    mat.row1.w = ExtractBits(ReadUShortBE(*fs));
    return mat;
}

void AnimUtils::GetAnimOrigin(std::istream* fs, Vec4* origin) {
    *origin = Vec4{ ReadFloatBE(*fs), ReadFloatBE(*fs),
        ReadFloatBE(*fs), ReadFloatBE(*fs) };
}

void AnimUtils::DecodeIKStream(std::istream* fs, uint16_t* numSegments, uint8_t* ikType, Matrix3x4* origin, std::vector<IKKey>* ikTransforms) {
    for (int k = 0; k < *numSegments; k++) {
        uint8_t numKeys = ReadByte(*fs);
        Vec4 transform;

        switch (*ikType) {
        case (0xD1):
            transform = AnimUtils::ShiftIKMatrix(fs, *origin);
            break;
        case (0xC0):
            break;
        }

        ikTransforms->push_back(IKKey{ transform,numKeys });
    }
}

void AnimUtils::ReadRotationMatrices(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations) {
    for (int k = 0; k < *numSegments; k++) {
        Matrix3x3 mat;
        uint16_t numKeys = ReadUShortBE(*fs);
        AnimUtils::StreamMatrix3x3(fs, &mat);
        rotations->push_back(MatrixKey{ mat, numKeys });
    }
}

void AnimUtils::DecodeEulerStreamS16(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection) {
    for (int k = 0; k < *numSegments; k++) {
        Vec3 transform = { URotToDegree * ReadShortBE(*fs),
            URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };

        uint16_t numKeys = ReadUShortBE(*fs);
        collection->push_back(TranslateKey{ transform, numKeys });
    }
}

void AnimUtils::DecodeDeltaStreamS16(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection) {
    for (int k = 0; k < *numSegments; k++) {
        Vec3 transform = { ReadShortBE(*fs),
             ReadShortBE(*fs),  ReadShortBE(*fs) };

        uint16_t numKeys = ReadUShortBE(*fs);
        collection->push_back(TranslateKey{ transform, numKeys });
    }
}

void AnimUtils::DecodeEulerStreamS8(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection) {
    for (int k = 0; k < *numSegments; k++) {
        Vec3 transform = { U8RotToDegree * ReadSignedByte(*fs),
            U8RotToDegree * ReadSignedByte(*fs), U8RotToDegree * ReadSignedByte(*fs) };

        uint8_t numKeys = ReadByte(*fs);
        collection->push_back(TranslateKey{ transform, numKeys });
    }
}

void AnimUtils::DecodeRotationStream8S(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations) {
    for (int k = 0; k < *numSegments; k++) {
        Matrix3x3 mat;
        mat.row0 = { U8RotToDegree * ReadSignedByte(*fs),
            U8RotToDegree * ReadSignedByte(*fs), U8RotToDegree * ReadSignedByte(*fs) };

        uint16_t numKeys = ReadByte(*fs);
        rotations->push_back(MatrixKey{ mat, numKeys });
    }
}
