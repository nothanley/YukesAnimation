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

class RotationHelper{

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

        GetStreamMatrix_A(&mat_a,&streamA, seg_a.index );
        GetStreamMatrix(&mat_b, &streamB, seg_b.index, true);

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

        float x = stream->at(segIndex).transform.x;
        x = x * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 x_vec = TransformVector(&x, 0);

        float y = stream->at(segIndex).transform.y * (isSubVector ? TwoPi_128 : TwoPi_256);
        y = y * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 y_vec = TransformVector(&y, 1);

        float z = stream->at(segIndex).transform.z * (isSubVector ? TwoPi_128 : TwoPi_256);
        Vec4 z_vec = TransformVector(&z, 2);

        return MergeTransformVecs(&x_vec, &y_vec, &z_vec);
    }

    float ValueShift2(float* in) {
        float w = *in;
        float square = w * w;
        float xmm5 = square * -2.38899e-008;

        xmm5 = 2.75256e-006 + xmm5;
        xmm5 = square * xmm5; 
        xmm5 = -0.000198409 + xmm5;
        xmm5 = xmm5 * square;
        xmm5 = 0.00833333 + xmm5;
        xmm5 = xmm5 * square;
        xmm5 = -0.166667 + xmm5;
        xmm5 = xmm5 * square;

        xmm5 = w * (1 + xmm5);
        return xmm5;
    }

    float ValueShift( float* w) {

        float absW = *w;
        float xmm5 = absW * -0.00126249;

        xmm5 = 0.00667009 + xmm5;
        xmm5 = xmm5 * absW;
        xmm5 = -0.0170881 + xmm5;
        xmm5 = xmm5 * absW;
        xmm5 = 0.0308919 + xmm5;
        xmm5 = xmm5 * absW;
        xmm5 = -0.0501743 + xmm5;
        xmm5 = xmm5 * absW;
        xmm5 = 0.088979 + xmm5;
        xmm5 = xmm5 * absW;
        xmm5 = -0.214599 + xmm5;
        xmm5 = xmm5 * absW;

        float test = (1 - absW) > 0 ? (1 - absW) : 0.0;
        test = sqrtf(test);

        xmm5 = 1.5708 + xmm5;
        xmm5 = xmm5 * test;
        return xmm5;
    }

    Vec4 CalcSomeVal(const float& input, const Vec4& vec) {
        float square = input * input;

        float oper_a = square * -2.38899e-008;
        oper_a = oper_a + 2.75256e-006;
        oper_a = oper_a * square;
        oper_a = -0.000198409 + oper_a;
        oper_a = oper_a * square;
        oper_a = 0.00833333 + oper_a;
        oper_a = oper_a * square;
        oper_a = -0.166667 + oper_a;
        oper_a = oper_a * square;
        oper_a = input * (1 + oper_a);

        float oper_b = square * -2.60516e-007;
        oper_b = oper_b + 2.47605e-005;
        oper_b = oper_b * square;
        oper_b = oper_b + -0.00138884;
        oper_b = oper_b * square;
        oper_b = oper_b + 0.0416666;
        oper_b = oper_b * square;
        oper_b = -0.5 + oper_b;
        oper_b = 1.0 + (oper_b * square);

        Vec4 vec_0 = vec * (oper_a / input);
        vec_0.w = oper_b;
        return vec_0;
    }

    Vec4 FlattenStreamVector(const Vec4& vec) {

        Vec4 square = vec * vec;
        Vec4 test =  Vec4{ 0.0 , square.z, 0.0 , 0.0 } + square ;
        test = Vec4{ 0.0 , square.y, 0.0 , 0.0 } + test;
        float sqrt_a = sqrtf(test.x);

        return CalcSomeVal(sqrt_a, vec);
    }

    Vec4 TransformValues(const Vec4& r9, const Vec4& rcx, const Vec4& r8, const Vec4& rdx) {

        /* todo: verify order of values after completion */
        Vec4 square_row_0 = r8 * r8;
        Vec4 sum_rows = square_row_0 + Vec4{ square_row_0.y, r8.z, r8.z, square_row_0.z };
        Vec4 sum_rows2 = sum_rows + Vec4{ sum_rows.x, square_row_0.z, square_row_0.z, sum_rows.w };

        Vec4 test = r8 * Vec4{ 1,-1,-1,-1 };
        test = test / sum_rows2.z;

        Vec4 test2 = Vec4{ rcx.z, rcx.w, rcx.x, rcx.y } *test.z;
        test2 = test2 * Vec4{ -1,1,-1,1 };

        Vec4 test3 = Vec4{ rcx.z, rcx.y, rcx.x, rcx.w } *test.z;
        test3 = test3 * Vec4{ -1,-1,1,1 };

        Vec4 test4 = Vec4{ rcx.y, rcx.x, rcx.w, rcx.z } *test.y;
        test4 = test4 * Vec4{ -1, 1,1,-1 };

        Vec4 test5 = rcx * test.w;
        Vec4 sumtest1 = test4 + test3;
        test5 = test5 + test2;
        sumtest1 = sumtest1 + test5;

        float w = ValueShift(&sumtest1.w);
        float w2 = ValueShift2(&w);
        float divRes = w / w2;
        Vec4 result_top = Vec4{ 0, sumtest1.x, sumtest1.y, sumtest1.z } *divRes;

        //////////////////////////////////// Next Row //////////////////////////////////////

        Vec4 later0 = r8 * Vec4{ 1, -1,-1,-1 };
        Vec4 laterxmm = later0 / 1.0 /* this 1 might be wrong */;

        Vec4 lTest0 = Vec4{ r9.x, r9.w, r9.z, r9.y } * laterxmm.z;
        lTest0 = lTest0 * Vec4{ -1,1,-1,1 };

        Vec4 ltest3 = Vec4{ r9.z, r9.y, r9.x, r9.w } * laterxmm.z;
        ltest3 = ltest3 * Vec4{ -1,-1,1,1 };

        Vec4 ltest4 = Vec4{ r9.y, r9.z, r9.w, r9.x } * laterxmm.y;
        ltest4 = ltest4 * Vec4{ -1, 1,1,-1 };

        Vec4 testv0 = r9 * laterxmm.w;
        Vec4 sum = (ltest3 + ltest4) + (testv0 + lTest0);

        float rw_0 = ValueShift(&sum.w);
        float rw_1 = ValueShift2(&rw_0);
        float divRes2 = rw_0 / rw_1;
        Vec4 result_bottom = Vec4{ 0, sum.x, sum.y, sum.z } *divRes2;

        //////////////////////////////////// continue this //////////////////////////////////////

        Vec4 total_result = result_top + result_bottom;
        total_result = total_result * -0.25;

        Vec4 flat_a = FlattenStreamVector(total_result);

        Vec4 xmm5 = r8;
        Vec4 res_3 = Vec4{ flat_a.x, flat_a.w, flat_a.z, flat_a.y } *r8.x;
        res_3 = res_3 * Vec4{ -1,1,-1,1 };
        Vec4 res_2 = Vec4{ flat_a.z, flat_a.y, flat_a.x, flat_a.w } *r8.z; // is this x??
        res_2 = res_2 * Vec4{ -1,-1,1,1 };
        Vec4 res_1 = Vec4{ flat_a.y, flat_a.z, flat_a.w, flat_a.x } *r8.y;
        res_1 = res_1 * Vec4{ -1,1,1,-1 };

        Vec4 res_0 = flat_a * r8.w;
        res_0 = res_0 + res_3;
        res_2 = res_2 + res_1;
        res_3 = res_0 + res_2;

        return res_3;
    }

    void MutateMatrices(Matrix4x4* mat) {

        /* Checks sign value of row's z value*/
        Vec4 val_r_3 = mat->row0 * mat->row2;
        Vec4 r3_sum = val_r_3 + Vec4{ val_r_3.y, mat->row2.x, mat->row2.x, val_r_3.x };
        val_r_3 = r3_sum + Vec4{ r3_sum.x, val_r_3.z, val_r_3.z, r3_sum.w };
        if (val_r_3.z < 0) { mat->row0 = 0.0 - mat->row0;  }


        Vec4 val_r_2 = mat->row0 * mat->row1;
        Vec4 r2_sum = val_r_2 + Vec4{ val_r_2.y, mat->row0.z, mat->row0.z, val_r_2.z };
        val_r_2 = r2_sum + Vec4{ r2_sum.x, val_r_2.z, val_r_2.z, r2_sum.w };
        if (val_r_3.z < 0) { mat->row1 = 0.0 - mat->row1;  }


        Vec4 val_r_1 = mat->row1 * mat->row3;
        Vec4 r1_sum = val_r_1 + Vec4{ val_r_1.y, mat->row1.x, mat->row1.x, val_r_1.x };
        val_r_1 = r1_sum + Vec4{ r1_sum.x, val_r_1.x, val_r_1.x, r1_sum.w };
        if (val_r_1.z < 0) {  mat->row3 = 0.0 - mat->row3; }

        Vec4 newRow = TransformValues( mat->row1, mat->row2, mat->row0, mat->row2 );
        Vec4 newRow2 = TransformValues( mat->row3, mat->row0, mat->row1, mat->row0 );

        *mat = Matrix4x4{ mat->row0, mat->row1, newRow, newRow2 };

    }


    void GetStreamMatrix_A(Matrix4x4* mat, const std::vector<TranslateKey>* stream, const unsigned int& index ) {
        mat->row0 = GetStreamVector(index, stream);
        mat->row1 = mat->row0;
        mat->row2 = mat->row0;

        if (index + 1 < stream->size()) {
            mat->row1 = GetStreamVector(index + 1, stream); }

        mat->row3 = mat->row1;
        if (index == 0) { return; }

        mat->row2 = GetStreamVector(index - 1, stream);

        if (index + 2 < stream->size()) {
            mat->row3 = GetStreamVector(index + 2, stream); }

        MutateMatrices(mat);
    }



    void GetStreamMatrix(Matrix4x4* mat, const std::vector<TranslateKey>* stream, const unsigned int& index, const bool isSubStream=false ) {
        mat->row0 = GetStreamVector(index, stream, isSubStream);
        mat->row1 = mat->row0;
        mat->row2 = mat->row0;

        if (index + 1 < stream->size()) {
            mat->row1 = GetStreamVector(index + 1, stream, isSubStream); }

        mat->row3 = mat->row1;
        if (index == 0 || isSubStream ) { return; }

        mat->row2 = GetStreamVector(index - 1, stream, isSubStream);
        if (index + 2 < stream->size()) {
            mat->row3 = GetStreamVector(index + 2, stream, isSubStream);  }
    }




};

