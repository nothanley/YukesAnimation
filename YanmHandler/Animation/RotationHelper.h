#pragma once
#include "AnimationUtils.h"
#include "../QuatEulerConv.h"

const float TwoPi_256 = 0.0245437; // 2pi/256
const float TwoPi_128 = 0.000681769; // 2pi/128

struct SegmentStats {
    int index = -1;
    int begin = -1;
    int length = 0;
};

class RotationHelper{

public:
    std::vector<MatrixKey> unpackedTransforms;
    unsigned int trackLength = 0;
    unsigned int frameStep = 1;
    uint32_t boneHash = 0;
    unsigned int numBits = 0;

    RotationHelper( const std::vector<TranslateKey>& vec_a, const std::vector<TranslateKey>& vec_b,
                    const uint32_t& hash, const unsigned int& numBits ) {

        AnimUtils::GetVectorRuntime(vec_a, &trackLength);
        this->numBits = numBits;
        this->streamA = vec_a;
        this->streamB = vec_b;
        this->boneHash = hash;
        if (streamB.size() > 0) { this->hasSubSegment = true; }

        // Unpack all stream transforms
        Deconstruct();
    }

private:
    bool hasSubSegment = false;
    unsigned int sceneKeyNum = 0;
    std::vector<TranslateKey> streamA;
    std::vector<TranslateKey> streamB;
    SegmentStats seg_a;
    SegmentStats seg_b;

    void Deconstruct() {

        for (sceneKeyNum = frameStep; sceneKeyNum < trackLength; sceneKeyNum += frameStep) {
            InitializeSegment(sceneKeyNum, &this->seg_a, &streamA);
            if (hasSubSegment) { InitializeSegment(sceneKeyNum, &this->seg_b, &streamB); }
            ReadValues();     }

    }

    void InitializeSegment( const unsigned int sceneFrame, SegmentStats* segment, const std::vector<TranslateKey>* stream ) {
        // Initialize if empty counter
        if (segment->index == -1) { segment->begin = 0;}

        // If track keyframe is longer than our current segment spans, move to next
        if (sceneFrame > (segment->begin + segment->length)) {
            segment->begin += segment->length;
            segment->index++;
            segment->length = stream->at(segment->index + 1).duration;  } // update key length
    }

    void ReadValues() {
        if (boneHash == 0xF91A7A26 && sceneKeyNum >= 0xF4)
            printf("");
       
        Matrix4x4 mat_a = GetStreamMatrix(&streamA, seg_a.index);
        Matrix4x4 mat_b = GetStreamMatrix(&streamB, seg_b.index, true);

        // Calculate interpolation from deltas
        float interpolationLevel_a = float(sceneKeyNum - seg_a.begin) / float(seg_a.length);
        float interpolationLevel_b = float(sceneKeyNum - seg_b.begin) / float(seg_b.length);

        // Get transforms for top and bottom streams:
        Vec4 transform_a, transform_b, jointQuat;
        transform_a = GetTopLevelVector(&mat_a, interpolationLevel_a);

        // Unpacks/combines all vectors and outputs it's quaternion transform
        if (hasSubSegment) {
            transform_b = FlattenMatrix(&mat_b.row0, &mat_b.row1, &interpolationLevel_b);
            jointQuat = MergeFlattenedVectors(&transform_a, &transform_b);   }
        else {
            jointQuat = ConvertVectorToQuat(transform_a);      }

        // Append quat as euler to our scene array
        this->unpackedTransforms.push_back( GetUnpackedEuler(jointQuat) );
    }

    MatrixKey GetUnpackedEuler(const Vec4& quaternion) {
        MatrixKey eulerKey;
        eulerKey.transform.row0 = QuatEulerConv::QuaternionToEuler(quaternion); /* ZYX Ordered Euler */
        eulerKey.duration = this->frameStep;
        return eulerKey;
    }

    void Read8BitVectors( Vec4* matrixRow, const int& segIndex, const std::vector<TranslateKey>* stream, const bool& isSubVector = false ) {

        float x = stream->at(segIndex).transform.x * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 x_vec = TransformVector(&x, 0);

        float y = stream->at(segIndex).transform.y * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 y_vec = TransformVector(&y, 1);

        float z = stream->at(segIndex).transform.z * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 z_vec = TransformVector(&z, 2);

        *matrixRow = MergeTransformVecs(&x_vec, &y_vec, &z_vec);
    }

    void GetStreamVector( Vec4* matrixRow, const unsigned int& segIndex, const std::vector<TranslateKey>* stream, const bool& isSubVector = false ) {

        switch (this->numBits) {
            case 16:
                Read8BitVectors(matrixRow, segIndex, stream, isSubVector);
                break;
            case 8:
                Read8BitVectors(matrixRow, segIndex, stream, isSubVector);
                break;
            default:
                break;
        }

    }


    Matrix4x4 GetStreamMatrix(const std::vector<TranslateKey>* stream, const unsigned int index, bool isSubStream=false) {
        Matrix4x4 mat{};
        if (stream->size() == 0) return mat;

        // Initialize matrix with first row vector
        GetStreamVector( &mat.row0, index, stream, isSubStream );
        mat = Matrix4x4{ mat.row0, mat.row0, mat.row0, mat.row0 };

        // Collect row 1 vector and copy to row3
        if ( index + 1 < stream->size() ) { 
            GetStreamVector(&mat.row1, index+1, stream, isSubStream ); }
        mat.row3 = mat.row1;

        // If stream has sufficient values, read and populate row 2
        if ( index == 0 || isSubStream ) return mat;
        GetStreamVector(&mat.row2, index - 1, stream, isSubStream);

        // Read and populate row 3
        if ( index + 2 < stream->size() ) {
            GetStreamVector(&mat.row3, index+2, stream, isSubStream );  }

        // Transform all vectors and output non interpolated matrix
        if (!isSubStream){
            MutateMatrices(&mat); }

        return mat;
    }




};

