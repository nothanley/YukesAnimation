#pragma once
#include "Animation/AnimationUtils.h"
using namespace BinaryIO;

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
	RotationHelper( std::vector<TranslateKey> vec_a , std::vector<TranslateKey> vec_b ) {
        AnimUtils::GetVectorRuntime(vec_a, &trackLength);
        this->streamA = vec_a;
        this->streamB = vec_b;
        Deconstruct(); 	}

private:

    unsigned int trackLength = 0;
    unsigned int sceneKeyNum = 0;
	std::vector<TranslateKey> streamA;
	std::vector<TranslateKey> streamB;
    SegmentStats seg_a;
    SegmentStats seg_b;

    void Deconstruct() {

        for (this->sceneKeyNum = 2; sceneKeyNum < 3 /*track length*/; sceneKeyNum += 2) {
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


    }

    Vec4 GetTopLevelVector( Matrix4x4* mat , const float interpolation ) {
        float subInterp = (interpolation -
            (interpolation * interpolation)) * 2.0;

        Vec4 tfmTop = FlattenMatrix(&mat->row0, &mat->row1, &interpolation);
        Vec4 tfmBottom = FlattenMatrix(&mat->row2, &mat->row3, &interpolation);

        Vec4 flatVec = FlattenMatrix(&tfmBottom, &tfmTop, &subInterp);

        flatVec = flatVec + Vec4{ flatVec.w, flatVec.x, flatVec.y, flatVec.z };

        return flatVec;
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

    Vec4 FlattenMatrix(const Vec4* vec_a, const Vec4* vec_b, const float* interpolation) {

        Vec4 vec_mult = (*vec_a) * (*vec_b);
        Vec4 val_3 = Vec4{ vec_mult.y, vec_b->x, vec_b->x, vec_mult.x } + vec_mult;
        Vec4 val_4 = Vec4{ val_3.x, vec_mult.x, vec_mult.x, val_3.w } + val_3;
        Vec4 val_0 = sqrt(1 - (Vec4{ val_4.z, val_4.z, val_4.z, val_4.z } *val_4.z));

        Vec4 normal = NormalizeVector(Vec4(val_0 / val_4.z));
        normal = normal * Vec4{ 0, (-*interpolation) + 1, *interpolation, 0 };
        Vec4 normInt = InterpolateVector(normal) / val_0;

        return (*vec_a * normInt.x) + (*vec_b * normInt.y);
    }

    Vec4 InterpolateVector(Vec4 interpVec) {
        Vec4 square = interpVec * interpVec;
        Vec4 result = square * ((square * ((square * -2.38899e-008) + 2.75256e-006)) + -0.000198409);
        result = interpVec * ((square * ((square * (result + 0.00833333)) + -0.166667)) + 1.0);
        return result;
    }

    Vec4 NormalizeVector(Vec4 input) {

        Vec4 square = Vec4{ input.w,input.w,input.w,input.w } *input.w;

        Vec4 result = square * ((square * 0.00286623) + -0.0161657);

        result = square * ((square * (result + 0.0429096)) + -0.0752896);

        result = square * ((square * (result + 0.106563)) + -0.142089);

        result = square * ((square * (result + 0.199936)) + -0.333331);

        result = (result + 1) * input.w;

        return result;
    }


    Vec4 MergeTransformVecs(const Vec4* vec_a, const Vec4* vec_b, const Vec4* vec_c) {

        Vec4 tfm_a = Vec4{ vec_b->z, vec_b->y, vec_b->x, vec_b->w } * vec_c->z;       
        tfm_a = (*vec_b * vec_c->w) + (tfm_a * Vec4{ -1, -1, 1, 1 }) ;

        Vec4 tfm_b = Vec4{ vec_a->x, vec_a->w, vec_a->y, vec_a->z } * tfm_a.x;
        tfm_b = tfm_b * Vec4{ -1, 1, -1, 1 };

        Vec4 vec_1 = Vec4{ vec_a->z, vec_a->y, vec_a->x, vec_a->w } * tfm_a.z;
        vec_1 = vec_1 * Vec4{ -1, -1, 1, 1 };

        Vec4 vec_2 = Vec4{ vec_a->y, vec_a->z, vec_a->w, vec_a->x } * tfm_a.y;
        vec_2 = vec_2 * Vec4{ -1, 1, 1, -1 };

        Vec4 vec_3 = Vec4{ vec_a->w, vec_a->x, vec_a->y, vec_a->z } * tfm_a.w;

        return (vec_1 + vec_2) + (tfm_b + vec_3);
    }

    Vec4 TransformVector(float* val, const int index) {

        Vec4 output{};
        *val *= 0.5;

        Vec4 square = Vec4{ *val,*val,*val,*val } * Vec4{ *val,*val,*val,*val };
        Vec4 sineCoeff = (square*((square*((square*((square *
                2.74003e-006) + -0.000198409) )+ 0.00833333))+ -0.166667))+1.0;
        sineCoeff = sineCoeff * *val;

        Vec4 vec_b = square * ((square * -2.60516e-007) + 2.47605e-005);
        vec_b = square * ((square * (vec_b + -0.00138884)) + 0.0416666);
        vec_b = (square * (vec_b-0.5)) + 1.0;
        
        switch (index) {
            case 0:
                output = Vec4{ vec_b.w, sineCoeff.w, 0, 0};
                break;
            case 1:
                output = Vec4{ vec_b.w, 0, sineCoeff.w, 0 };
                break;
            case 2:
                output = Vec4{ vec_b.w, 0, 0, sineCoeff.w };
                break;
        }

        return output;
    }


};

