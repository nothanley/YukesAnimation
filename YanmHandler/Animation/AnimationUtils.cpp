#include "../DataIO/BinaryIO.h"
#include "AnimationUtils.h"
using namespace BinaryIO;

void AnimUtils::StreamMatrix3x3(std::istream* fs, Matrix3x3* mat){
	// float URotToDegree = 1.0; pretransform
	mat->row0 = Vec3{ U16RotToDegree * ReadShortBE(*fs), U16RotToDegree * ReadShortBE(*fs), U16RotToDegree * ReadShortBE(*fs) };
	mat->row1 = Vec3{ U16RotToDegree * ReadShortBE(*fs), U16RotToDegree * ReadShortBE(*fs), U16RotToDegree * ReadShortBE(*fs) };
	mat->row2 = Vec3{ U16RotToDegree * ReadShortBE(*fs), U16RotToDegree * ReadShortBE(*fs), U16RotToDegree * ReadShortBE(*fs) };
}

void AnimUtils::GetPackedMatrix3x3(std::istream* fs, Matrix3x3* mat) {
    mat->row0 = Vec3{ (float)ReadShortBE(*fs), (float)ReadShortBE(*fs), (float)ReadShortBE(*fs) };
    mat->row1 = Vec3{ (float)ReadShortBE(*fs), (float)ReadShortBE(*fs), (float)ReadShortBE(*fs) };
    mat->row2 = Vec3{ (float)ReadShortBE(*fs), (float)ReadShortBE(*fs), (float)ReadShortBE(*fs) };
}

Vec4 AnimUtils::UnpackMatrixVector(Matrix3x4 mat) {
    Vec4 out;
    out.z = (mat.row0.z * mat.row1.z) + mat.row2.z;
    out.y = (mat.row0.y * mat.row1.y) + mat.row2.y;
    out.x = (mat.row0.x * mat.row1.x) + mat.row2.x;
    out.w = (mat.row0.w * mat.row1.w) + mat.row2.w;
    out.x *= -1;
    return out;
}

Vec4 AnimUtils::Extract32bitOrigin(std::istream* fs) {
    std::vector<uint8_t> stream;
    for (int i = 0; i < 8; i++)
        stream.push_back(ReadByte(*fs));

    uint32_t value0 = (stream[0] << 0x8) | stream[4];
    uint32_t value1 = (stream[1] << 0x8) | stream[5];
    uint32_t value2 = (stream[2] << 0x8) | stream[6];
    uint32_t value3 = (stream[3] << 0x8) | stream[7];

    return Vec4{ (float)value3, (float)value2, (float)value1, (float)value0 };
}

Vec4 AnimUtils::Extract24bitOrigin(std::istream* fs) {
    std::vector<uint8_t> stream;
    for (int i = 0; i < 6; i++)
        stream.push_back(ReadByte(*fs));

    uint32_t value0 = (stream[4]>> 0x4) | (stream[0] << 0x4);
    uint32_t value1 = (stream[4] & 0xF) | (stream[1] << 0x4);
    uint32_t value2 = (stream[5]>> 0x4) | (stream[2] << 0x4);
    uint32_t value3 = (stream[0] & 0xF) | (stream[3] << 0x4) ;


    return Vec4{ (float)value3, (float)value2, (float)value1, (float)value0 };
}

Vec4 AnimUtils::Extract16bitOrigin(std::istream* fs) {
    std::vector<uint8_t> stream;
    for (int i = 0; i < 4; i++)
        stream.push_back(ReadByte(*fs));

    uint32_t value0 = stream[0];
    uint32_t value1 = stream[1];
    uint32_t value2 = stream[2];
    uint32_t value3 = stream[3];

    return Vec4{ (float)value3, (float)value2, (float)value1, (float)value0 };
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

Vec4 AnimUtils::ShiftIKMatrix(std::istream* fs, Matrix3x4 mat, uint8_t encodeSize ) {
    switch (encodeSize) {
    case 0xD1:
        mat.row0 = Extract32bitOrigin(fs);
        break;
    case 0xC9:
        mat.row0 = Extract24bitOrigin(fs);
        break;
    case 0xC1:
        mat.row0 = Extract16bitOrigin(fs);
        break;
    default:
        break;   }
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
        mat.row0 = { U16RotToDegree * ReadShortBE(*fs),
            U16RotToDegree * ReadShortBE(*fs), U16RotToDegree * ReadShortBE(*fs) };

        uint16_t numKeys = ReadUShortBE(*fs);
        rotations->push_back(MatrixKey{ mat, numKeys });
    }
}

void AnimUtils::Get16bSignedByteArray(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* translations) {
    for (int k = 0; k < *numSegments; k++) {
        Vec3 transform = { ReadShortBE(*fs),
            ReadShortBE(*fs), ReadShortBE(*fs) };

        uint16_t numKeys = ReadUShortBE(*fs);
        translations->push_back(TranslateKey{ transform, numKeys });
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
            transform = AnimUtils::ShiftIKMatrix(fs, *origin, *ikType);
            break;
        case (0xC9):
            transform = AnimUtils::ShiftIKMatrix(fs, *origin, *ikType);
            break;
        case (0xC1):
            transform = AnimUtils::ShiftIKMatrix(fs, *origin, *ikType);
            break;
        default:
            transform = Vec4{ 0,0,0,0 };
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


void AnimUtils::Get16bSignedMatrix(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations) {
    for (int k = 0; k < *numSegments; k++) {
        Matrix3x3 mat;
        uint16_t numKeys = ReadUShortBE(*fs);
        AnimUtils::GetPackedMatrix3x3(fs, &mat);
        rotations->push_back(MatrixKey{ mat, numKeys });
    }
}

void AnimUtils::DecodeEulerStreamS16(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection) {
    for (int k = 0; k < *numSegments; k++) {
        Vec3 transform = { U16RotToDegree * ReadShortBE(*fs),
            U16RotToDegree * ReadShortBE(*fs), U16RotToDegree * ReadShortBE(*fs) };

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
        Vec3 transform = { DegUnk * ReadSignedByte(*fs) * 0.5,
            DegUnk * ReadSignedByte(*fs) * 0.5, DegUnk* ReadSignedByte(*fs) * 0.5 };

        uint8_t numKeys = ReadByte(*fs);
        collection->push_back(TranslateKey{ transform, numKeys });
    }

}

void TransformVector(Vec3* vector) {
    *vector = *vector * Vec3{ 0.159155,0.159155,0.159155 };
}

void InterpretU8Byte(Vec3* vector) {

    *vector = *vector * Vec3{ 0.5,0.5,0.5 };
}

void ReadAndTransformByte(float* value) {

    Vec3 vector = Vec3{ *value,*value,*value };
    InterpretU8Byte( &vector );

}

Vec3 ReadVector3f(std::istream* fs) {

    float A_X = (float)ReadSignedByte(*fs) * DegUnk;
    float A_Y = (float)ReadSignedByte(*fs) * DegUnk;
    float A_Z = (float)ReadSignedByte(*fs) * DegUnk;
    uint8_t numKeys = ReadByte(*fs);

    Vec3 vector = Vec3{ A_X,A_Y,A_Z };
    vector = vector * Vec3{ 0.5,0.5,0.5 };

    /*ReadAndTransformByte(&A_X);
    ReadAndTransformByte(&A_Y);
    ReadAndTransformByte(&A_Z);*/

    return vector;
}

void AnimUtils::DebugDecodeEulerStreamS8(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection) {

    std::streampos pos = fs->tellg();

    for (int k = 0; k < *numSegments; k++) {

        // Grab first transform
        Vec3 vector_a = ReadVector3f(fs);

        // Messy implementation of runtime ops
        if ( (k + 1) > *numSegments) { break; }
        
        // Grab second transform
        pos = fs->tellg();

        Vec3 vector_b = ReadVector3f(fs);

        Vec3 test = vector_a * vector_b;

        // Seek back to first stream
        fs->seekg(pos);

    }

}


void AnimUtils::Get8bSignedByteArray(std::istream* fs, uint32_t* numSegments, std::vector<TranslateKey>* collection) {
    for (int k = 0; k < *numSegments; k++) {
        Vec3 transform = { ReadSignedByte(*fs),ReadSignedByte(*fs),ReadSignedByte(*fs) };

        uint8_t numKeys = ReadByte(*fs);
        collection->push_back(TranslateKey{ transform, numKeys });
    }
}

void AnimUtils::DecodeRotationStream8S(std::istream* fs, uint32_t* numSegments, std::vector<MatrixKey>* rotations) {
    for (int k = 0; k < *numSegments; k++) {
        Matrix3x3 mat;
        mat.row0 = { SByteToDeg * ReadByte(*fs),
            SByteToDeg * ReadByte(*fs), SByteToDeg * ReadByte(*fs) };

        uint8_t numKeys = ReadByte(*fs);
        rotations->push_back(MatrixKey{ mat, numKeys });
    }
}



void AnimUtils::ApplyVectorDeltas(std::vector<TranslateKey>* vector, Vec4* delta) {

    for (auto& key : *vector) {
        key.transform.x *= delta->w;
        key.transform.y *= delta->w;
        key.transform.z *= delta->w;}

    Vec3 mutatingDelta = Vec3{ delta->x,delta->y,delta->z };
    for (auto& key : *vector) {
        key.transform.x += mutatingDelta.x;
        key.transform.y += mutatingDelta.y;
        key.transform.z += mutatingDelta.z;
        mutatingDelta = key.transform;   }

}