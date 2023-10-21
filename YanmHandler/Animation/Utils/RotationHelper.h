#pragma once
#include "../AnimationUtils.h"
#include "../Utils/QuatEulerConv.h"

const float TWOPI_256 = 0.0245437; // 2pi/256
const float TWOPI_128 = 0.000681769; // 2pi/128
const float SHORTMULT = 9.58738e-005;

struct SegmentStats {
    int index = -1;
    int begin = -1;
    int length = 0;
};

class RotationHelper{

public:
    std::vector<MatrixKey> unpackedTransforms;
    uint32_t boneHash = 0;
    unsigned int trackLength = 0;
    unsigned int frameStep = 1;
    unsigned int numBits = 0;


    RotationHelper(const std::vector<TranslateKey>& vec_a,
        const uint32_t& hash, const unsigned int& numBits) {

        AnimUtils::GetVectorRuntime(vec_a, &trackLength);
        this->numBits = numBits;
        this->streamA = vec_a;
        this->boneHash = hash;

        DeconstructVectors(); // Unpack all stream transforms
    }

    RotationHelper( const std::vector<TranslateKey>& vec_a, const std::vector<TranslateKey>& vec_b,
                    const uint32_t& hash, const unsigned int& numBits ) {

        AnimUtils::GetVectorRuntime(vec_a, &trackLength);
        this->numBits = numBits;
        this->streamA = vec_a;
        this->streamB = vec_b;
        this->boneHash = hash;
        this->hasSubSegment = true;

        DeconstructVectors(); // Unpack all stream transforms
    }

    RotationHelper(const std::vector<MatrixKey>& rotationMatrix, const uint32_t& hash, const unsigned int& numBits) {
        AnimUtils::GetVectorRuntime(rotationMatrix, &trackLength);
        this->numBits = numBits;
        this->streamMatrix = rotationMatrix;
        this->boneHash = hash;

        DeconstructMatrices(); // Unpack all stream transforms
    }

private:
    bool hasSubSegment = false;
    unsigned int sceneKeyNum = 0;
    std::vector<TranslateKey> streamA;
    std::vector<TranslateKey> streamB;
    std::vector<MatrixKey> streamMatrix;
    SegmentStats seg_a;
    SegmentStats seg_b;

    void DeconstructMatrices() {

        for (sceneKeyNum = frameStep; sceneKeyNum < trackLength; sceneKeyNum += frameStep) {
            InitializeSegment(sceneKeyNum, &this->seg_a, &streamMatrix);
            ReadMatrixValues();  }

    }

    void DeconstructVectors() {

        for (sceneKeyNum = frameStep; sceneKeyNum < trackLength; sceneKeyNum += frameStep) {
            InitializeSegment(sceneKeyNum, &this->seg_a, &streamA);
            if (hasSubSegment) { InitializeSegment(sceneKeyNum, &this->seg_b, &streamB); }
            ReadVectorValues();  }

    }

    void ReadMatrixValues() {

        Matrix4x4 mat_a = GetStreamMatrix(streamMatrix, seg_a.index);
        float interpolationLevel_a = float(sceneKeyNum - seg_a.begin) / float(seg_a.length);

        Vec4 transform_a = GetTopLevelVector(&mat_a, interpolationLevel_a);
        Vec4 quaternion = ConvertVectorToQuat(transform_a);

        this->unpackedTransforms.push_back(GetUnpackedEuler(quaternion));

    }

    void ReadVectorValues() {

        Matrix4x4 mat_a = GetStreamMatrix(streamA, seg_a.index);
        Matrix4x4 mat_b = GetStreamMatrix(streamB, seg_b.index, true);

        // Calculate interpolation from deltas
        float interpolationLevel_a = float(sceneKeyNum - seg_a.begin) / float(seg_a.length);
        float interpolationLevel_b = float(sceneKeyNum - seg_b.begin) / float(seg_b.length);

        // Get transforms for top and bottom streams:
        Vec4 transform_a, transform_b, quaternion;
        transform_a = GetTopLevelVector(&mat_a, interpolationLevel_a);

        // Unpacks/combines all vectors and outputs it's quaternion transform
        if (hasSubSegment) {
            transform_b = FlattenMatrix(&mat_b.row0, &mat_b.row1, &interpolationLevel_b);
            quaternion = MergeFlattenedVectors(&transform_a, &transform_b);   }
        else {
            quaternion = ConvertVectorToQuat(transform_a);      }

        // Append quat as euler to our scene array
        this->unpackedTransforms.push_back( GetUnpackedEuler(quaternion) );

    }

    MatrixKey GetUnpackedEuler(const Vec4& quaternion) {
        MatrixKey eulerKey;
        eulerKey.transform.row0 = QuatEulerConv::QuaternionToEuler(quaternion); /* ZYX Ordered Euler */
        eulerKey.duration = this->frameStep;
        return eulerKey;
    }

    template <typename KeyType>
    void InitializeSegment(const unsigned int sceneFrame, SegmentStats* segment, const std::vector<KeyType>* stream) {
        // Initialize if empty counter
        if (segment->index == -1) { segment->begin = 0; }

        // If track keyframe is longer than our current segment spans, move to next
        if (sceneFrame > (segment->begin + segment->length)) {
            segment->begin += segment->length;
            segment->index++;
            segment->length = stream->at(segment->index + 1).duration;
        }
    }

    Vec4 Read16BitVectors(const Vec3& streamVec, const std::vector<MatrixKey>& stream, const bool& isSubVector = false) {
        float x = streamVec.x * SHORTMULT;
        Vec4 x_vec = TransformVector(&x, 0);

        float y = streamVec.y * SHORTMULT;
        Vec4 y_vec = TransformVector(&y, 1);

        float z = streamVec.z * SHORTMULT;
        Vec4 z_vec = TransformVector(&z, 2);

        return MergeTransformVecs(&x_vec, &y_vec, &z_vec);
    }


    Matrix4x4 GetStreamMatrix(const std::vector<MatrixKey>& stream, const unsigned int index, bool isSubStream = false) {
        Matrix4x4 mat{};
        if (stream.size() == 0) return mat;
        
        mat.row0 = Read16BitVectors(stream.at(index).transform.row0, stream);
        mat.row2 = Read16BitVectors(stream.at(index).transform.row2, stream);

        if ( index + 1 < stream.size() ) {
            mat.row1 = Read16BitVectors(stream.at(index + 1).transform.row0, stream);
            mat.row3 = Read16BitVectors(stream.at(index + 1).transform.row1, stream);
        }
        else {
            mat.row1 = mat.row0;
            mat.row3 = mat.row2;
        }

        return mat;
    }


    void Read8BitVectors(Vec4* matrixRow, const int& segIndex, const std::vector<TranslateKey>& stream, const bool& isSubVector = false) {
        float x = stream.at(segIndex).transform.x * (isSubVector ? TWOPI_128 : TWOPI_256);
        Vec4 x_vec = TransformVector(&x, 0);

        float y = stream.at(segIndex).transform.y * (isSubVector ? TWOPI_128 : TWOPI_256);
        Vec4 y_vec = TransformVector(&y, 1);

        float z = stream.at(segIndex).transform.z * (isSubVector ? TWOPI_128 : TWOPI_256);
        Vec4 z_vec = TransformVector(&z, 2);

        *matrixRow = MergeTransformVecs(&x_vec, &y_vec, &z_vec);
    }

    template <typename KeyType>
    void GetStreamVector(Vec4* matrixRow, const unsigned int& segIndex, const std::vector<KeyType>& stream, const bool& isSubVector = false) {

        switch (this->numBits) {
            case 16:
                Read16BitVectors(matrixRow, segIndex, stream, isSubVector);
                break;
            case 8:
                Read8BitVectors(matrixRow, segIndex, stream, isSubVector);
                break;
            default:
                break;
        }

    }

    Matrix4x4 GetStreamMatrix(const std::vector<TranslateKey>& stream, const unsigned int index, bool isSubStream=false) {
        Matrix4x4 mat{};
        if (stream.size() == 0) return mat;

        // Initialize matrix with first row vector
        GetStreamVector( &mat.row0, index, stream, isSubStream );
        mat = Matrix4x4{ mat.row0, mat.row0, mat.row0, mat.row0 };

        // Collect row 1 vector and copy to row3
        if ( index + 1 < stream.size() ) { 
            GetStreamVector(&mat.row1, index+1, stream, isSubStream ); }
        mat.row3 = mat.row1;

        // If stream has sufficient values, read and populate row 2
        if ( index == 0 || isSubStream ) return mat;
        GetStreamVector(&mat.row2, index - 1, stream, isSubStream);

        // Read and populate row 3
        if ( index + 2 < stream.size() ) {
            GetStreamVector(&mat.row3, index+2, stream, isSubStream );  }

        // Transform all vectors and output non interpolated matrix
        if (!isSubStream){
            MutateMatrices(&mat); }

        return mat;
    }

   
    void Read16BitVectors(Vec4* matrixRow, const int& segIndex, const std::vector<TranslateKey>& stream, const bool& isSubVector = false) {
        float x = stream.at(segIndex).transform.x * SHORTMULT;
        Vec4 x_vec = TransformVector(&x, 0);

        float y = stream.at(segIndex).transform.y * SHORTMULT;
        Vec4 y_vec = TransformVector(&y, 1);

        float z = stream.at(segIndex).transform.z * SHORTMULT;
        Vec4 z_vec = TransformVector(&z, 2);

        *matrixRow = MergeTransformVecs(&x_vec, &y_vec, &z_vec);
    }


    // TBD unused vecs
    void Read8BitVectors(Vec4* matrixRow, const int& segIndex, const std::vector<MatrixKey>& stream, const bool& isSubVector = false) {}



};

