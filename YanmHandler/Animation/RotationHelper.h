#pragma once
#include "AnimationUtils.h"
#include "../QuatEulerConv.h"

const float TwoPi_256 = 0.0245437;
const float TwoPi_128 = 0.000681769;

struct SegmentStats {
    int index = -1;
    int begin = -1;
    int length = 0;
};

class RotationHelper
{

public:
    std::vector<MatrixKey> unpackedTransforms;
    unsigned int trackLength = 0;

	RotationHelper( std::vector<TranslateKey> vec_a , std::vector<TranslateKey> vec_b ) {
        AnimUtils::GetVectorRuntime(vec_a, &trackLength);
        this->streamA = vec_a;
        this->streamB = vec_b;
        Deconstruct(); 	}

private:
    unsigned int sceneKeyNum = 0;
	std::vector<TranslateKey> streamA;
	std::vector<TranslateKey> streamB;
    SegmentStats seg_a;
    SegmentStats seg_b;

    void Deconstruct() {

        for (this->sceneKeyNum = 2; sceneKeyNum < trackLength /*track length*/; sceneKeyNum += 2) {
            InitializeSegment(sceneKeyNum, &this->seg_a, &streamA);
            InitializeSegment(sceneKeyNum, &this->seg_b, &streamB);
            ReadValues();
        }

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
    
        Matrix4x4 mat_a;
        Matrix4x4 mat_b;

        GetStreamMatrix(&mat_a,&streamA);
        GetStreamMatrix(&mat_b, &streamB,true);

        // Calculate interpolation from deltas
        float interpolationLevel_a = float(sceneKeyNum - seg_a.begin) / float(seg_a.length);
        float interpolationLevel_b = float(sceneKeyNum - seg_b.begin) / float(seg_b.length);

        // Get transforms for top and bottom streams:
        Vec4 transform_a = GetTopLevelVector(&mat_a, interpolationLevel_a);
        Vec4 transform_b = FlattenMatrix(&mat_b.row0, &mat_b.row1, &interpolationLevel_b);

        // Unpacks/combines all vectors and outputs it's quaternion transform
        Vec4 jointQuat = MergeFlattenedVectors(&transform_a, &transform_b);

        // Append quat as euler to our scene array
        MatrixKey eulerKey;
        eulerKey.transform.row0 = QuatEulerConv::QuaternionToEuler(jointQuat); /* ZYX Ordered Euler*/
        eulerKey.duration = 2;

        this->unpackedTransforms.push_back(eulerKey);
    }


    Vec4 GetStreamVector(int segIndex, const std::vector<TranslateKey>* stream, bool isSubVector = false) {

        float x = stream->at(segIndex).transform.x * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 x_vec = TransformVector(&x, 0);

        float y = stream->at(segIndex).transform.y * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 y_vec = TransformVector(&y, 1);

        float z = stream->at(segIndex).transform.z * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 z_vec = TransformVector(&z, 2);

        return MergeTransformVecs(&x_vec, &y_vec, &z_vec);
    }


    void GetStreamMatrix(Matrix4x4* mat, const std::vector<TranslateKey>* stream, bool isSubStream=false ) {
        mat->row0 = GetStreamVector(seg_a.index, stream, isSubStream);
        mat->row1 = mat->row0;
        mat->row2 = mat->row0;

        if (seg_a.index + 1 < stream->size()) {
            mat->row1 = GetStreamVector(seg_a.index + 1, stream, isSubStream); }

        mat->row3 = mat->row1;
        if (seg_a.index == 0 || isSubStream ) { return; }

        mat->row2 = GetStreamVector(seg_a.index - 1, stream, isSubStream);
        if (seg_a.index + 2 < stream->size()) {
            mat->row3 = GetStreamVector(seg_a.index + 2, stream, isSubStream);  }
    }




};

