#pragma once
#include <windows.h> 
#include <cmath>

struct Vec4 {
    float w, x, y, z;
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

static Vec4 sqrt(const Vec4& vec_a) {
    Vec4 result = { sqrtf(vec_a.w),
                    sqrtf(vec_a.x),
                    sqrtf(vec_a.y),
                    sqrtf(vec_a.z) };

    if (std::isnan(sqrtf(vec_a.w)))
        result = Vec4{ 0,0,0,0 };

    return result;
}

static bool isNan(const Vec4& vec_a) {

    if (std::isnan(vec_a.x)) { return true; }
    if (std::isnan(vec_a.y)) { return true; }
    if (std::isnan(vec_a.z)) { return true; }
    if (std::isnan(vec_a.w)) { return true; }

    return false;
}

static bool isInf(const Vec4& vec_a) {
    
    if (std::isinf(vec_a.x)) { return true; }
    if (std::isinf(vec_a.y)) { return true; }
    if (std::isinf(vec_a.z)) { return true; }
    if (std::isinf(vec_a.w)) { return true; }

    return false;
}

static Vec4 operator*(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w * vec_b.w,
                    vec_a.x * vec_b.x,
                    vec_a.y * vec_b.y,
                    vec_a.z * vec_b.z };
    return result;
}

static Vec4 operator/(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w / vec_b.w,
                    vec_a.x / vec_b.x,
                    vec_a.y / vec_b.y,
                    vec_a.z / vec_b.z };
    return result;
}


static Vec4 operator+(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w + vec_b.w,
                    vec_a.x + vec_b.x,
                    vec_a.y + vec_b.y,
                    vec_a.z + vec_b.z };
    return result;
}

static Vec4 operator-(const Vec4& vec_a, const Vec4& vec_b) {
    Vec4 result = { vec_a.w - vec_b.w,
                    vec_a.x - vec_b.x,
                    vec_a.y - vec_b.y,
                    vec_a.z - vec_b.z, };
    return result;
}

static Vec4 operator-(const Vec4& vec_a, const float& sub) {
    Vec4 result = { vec_a.w - sub,
                    vec_a.x - sub,
                    vec_a.y - sub,
                    vec_a.z - sub };
    return result;
}

static Vec4 operator-(const float& sub, const Vec4& vec_a) {
    Vec4 result = { sub - vec_a.w,
                    sub - vec_a.x,
                    sub - vec_a.y,
                    sub - vec_a.z };
    return result;
}

static Vec4 operator+(const float& sub, const Vec4& vec_a) {
    Vec4 result = { sub + vec_a.w,
                    sub + vec_a.x,
                    sub + vec_a.y,
                    sub + vec_a.z };
    return result;
}

static Vec4 operator*(const Vec4& vec_a, const float& b) {
    Vec4 result = { vec_a.w * b,
                    vec_a.x * b,
                    vec_a.y * b,
                    vec_a.z * b };
    return result;
}

static Vec4 operator+(const Vec4& vec_a, const float& b) {
    Vec4 result = { vec_a.w + b,
                    vec_a.x + b,
                    vec_a.y + b,
                    vec_a.z + b };
    return result;
}

static Vec4 operator/(const float& sub, const Vec4& vec_a) {
    Vec4 result = { sub / vec_a.w,
                    sub / vec_a.x,
                    sub / vec_a.y,
                    sub / vec_a.z };
    return result;
}

static Vec4 operator/(const Vec4& vec_a, const float& sub) {
    Vec4 result = { vec_a.w / sub,
                    vec_a.x / sub,
                    vec_a.y / sub,
                    vec_a.z / sub };
    return result;
}

static double round_to(double value, double precision = 1.0)
{
    return std::round(value / precision) * precision;
}

inline Vec4 NormalizeVector(Vec4 input) {

    Vec4 square = Vec4{ input.w,input.w,input.w,input.w } *input.w;
    Vec4 result = square * ((square * 0.00286623) + -0.0161657);
    result = square * ((square * (result + 0.0429096)) + -0.0752896);
    result = square * ((square * (result + 0.106563)) + -0.142089);
    result = square * ((square * (result + 0.199936)) + -0.333331);
    result = (result + 1) * input.w;

    return result;
}

inline Vec4 InterpolateVector(Vec4 interpVec) {
    Vec4 square = interpVec * interpVec;

    Vec4 result = (square * -2.38899e-008);

    result = result + 2.75256e-006;
    result = result * square;

    result = result + -0.000198409;
    result = result * square;

    result = result + 0.00833333;
    result = result * square;

    result = result + -0.166667;
    result = result * square;
    result = 1.0 + result;

    result = interpVec * result;
    return result;
}


inline Vec4 FlattenMatrix(const Vec4* vec_a, const Vec4* vec_b, const float* interpolation) {

    Vec4 vec_mult = (*vec_a) * (*vec_b);
    Vec4 val_3 = Vec4{ vec_mult.y, vec_b->x, vec_b->x, vec_mult.x } + vec_mult;
    Vec4 val_4 = Vec4{ val_3.x, vec_mult.x, vec_mult.x, val_3.w } + val_3;
    float sqrtVal = ( 1.0 - ( double(val_4.z) * double(val_4.z) ) );
    //sqrtVal = (sqrtVal < 0) ? 0.0 : sqrtVal;
    sqrtVal = sqrt(sqrtVal);

    // Fix this problematic code...
    Vec4 normal = NormalizeVector(Vec4(sqrtVal / -val_4.z));
    normal = normal * Vec4{ 0, (-*interpolation) + 1, *interpolation, 0 };
    Vec4 normInt = InterpolateVector(normal) / Vec4{ sqrtVal,sqrtVal,sqrtVal,sqrtVal };
    if ( isNan(normInt) || isInf(normInt) ) { normInt = Vec4{ 0,0,0,0 };    }

    normInt.x = -(normInt.x);
    normInt.y = -(normInt.y);

    return  (*vec_a * normInt.x) + (*vec_b * normInt.y);
}

inline Vec4 GetTopLevelVector(Matrix4x4* mat, const float interpolation) {
    float subInterp = (interpolation -
        (interpolation * interpolation)) * 2.0;

    Vec4 tfmBottom = FlattenMatrix(&mat->row2, &mat->row3, &interpolation);
    Vec4 tfmTop = FlattenMatrix(&mat->row0, &mat->row1, &interpolation);

    Vec4 flatVec = FlattenMatrix(&tfmTop, &tfmBottom, &subInterp);

    Vec4 square = flatVec * flatVec;
    Vec4 vec_lin = square + Vec4{ square.w, square.z, square.w, square.z };
    Vec4 vec_sum = Vec4{ vec_lin.y, vec_lin.x, vec_lin.x, vec_lin.x } +
        Vec4{ vec_lin.y, square.z, square.z, vec_lin.y };

    Vec4 result = flatVec / sqrt(Vec4{ vec_sum.z, vec_sum.z, vec_sum.z, vec_sum.z });
    return result;
}

inline Vec4 MergeFlattenedVectors(const Vec4* transform_a, const Vec4* transform_b) {

    Vec4 pack_x = Vec4{ transform_b->x, transform_b->w,
        transform_b->z, transform_b->y } *transform_a->x;
    pack_x = pack_x * Vec4{ -1 , 1, -1, 1 };
    Vec4 pack_y = Vec4{ transform_b->z, transform_b->y,
       transform_b->x, transform_b->w } *transform_a->z;
    pack_y = pack_y * Vec4{ -1 , -1, 1, 1 };
    Vec4 pack_z = Vec4{ transform_b->y, transform_b->z,
        transform_b->w, transform_b->x } *transform_a->y;
    pack_z = pack_z * Vec4{ -1 , 1, 1, -1 };
    Vec4 pack_w = (*transform_b) * (transform_a->w);
    Vec4 unpacked_vec = (pack_x + pack_w) + (pack_z + pack_y);

    // Output quaternion vector
    Vec4 square = unpacked_vec * unpacked_vec;
    Vec4 tfm = Vec4{ square.w, square.z, square.w, square.z } + (square);
    Vec4 result = Vec4{ tfm.y, square.z, square.z, tfm.y } + Vec4{ tfm.y, tfm.x, tfm.x, tfm.x };
    result = sqrt(Vec4{ result.z, result.z, result.z, result.z });

    return (unpacked_vec / result);
}

inline Vec4 MergeTransformVecs(const Vec4* vec_a, const Vec4* vec_b, const Vec4* vec_c) {

    Vec4 tfm_a = Vec4{ vec_b->z, vec_b->y, vec_b->x, vec_b->w } *vec_c->z;
    tfm_a = (*vec_b * vec_c->w) + (tfm_a * Vec4{ -1, -1, 1, 1 });

    Vec4 tfm_b = Vec4{ vec_a->x, vec_a->w, vec_a->y, vec_a->z } *tfm_a.x;
    tfm_b = tfm_b * Vec4{ -1, 1, -1, 1 };

    Vec4 vec_1 = Vec4{ vec_a->z, vec_a->y, vec_a->x, vec_a->w } *tfm_a.z;
    vec_1 = vec_1 * Vec4{ -1, -1, 1, 1 };

    Vec4 vec_2 = Vec4{ vec_a->y, vec_a->z, vec_a->w, vec_a->x } *tfm_a.y;
    vec_2 = vec_2 * Vec4{ -1, 1, 1, -1 };

    Vec4 vec_3 = Vec4{ vec_a->w, vec_a->x, vec_a->y, vec_a->z } *tfm_a.w;

    return (vec_1 + vec_2) + (tfm_b + vec_3);
}

inline Vec4 TransformVector(float* val, const int index) {

    Vec4 output{};
    *val *= 0.5;

    Vec4 square = Vec4{ *val,*val,*val,*val } *Vec4{ *val,*val,*val,*val };
    Vec4 sineCoeff = (square * ((square * ((square * ((square *
        2.74003e-006) + -0.000198409)) + 0.00833333)) + -0.166667)) + 1.0;
    sineCoeff = sineCoeff * *val;

    Vec4 vec_b = square * ((square * -2.60516e-007) + 2.47605e-005);
    vec_b = square * ((square * (vec_b + -0.00138884)) + 0.0416666);
    vec_b = (square * (vec_b - 0.5)) + 1.0;

    switch (index) {
    case 0:
        output = Vec4{ vec_b.w, sineCoeff.w, 0, 0 };
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


inline float ValueShift2(float* in) {
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

inline float ValueShift(float* w) {

    float absW = round_to(*w, 0.0000001);
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

    float test = (1.0 - absW) > 0 ? (1.0 - absW) : 0.0; //
    test = sqrtf(test); //?

    xmm5 = 1.5708 + xmm5;
    xmm5 = xmm5 * test;
    return xmm5;
}

inline Vec4 CalcSomeVal(const float& input, const Vec4& vec) {
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

    float divisor = std::isnan(oper_a / input) ? 0.0 : (oper_a / input);
    Vec4 vec_0 = vec * divisor;
    vec_0.w = oper_b;
    return vec_0;
}

inline Vec4 FlattenStreamVector(const Vec4& vec) {

    Vec4 square = vec * vec;
    Vec4 test = Vec4{ 0.0 , square.z, 0.0 , 0.0 } + square;
    test = Vec4{ 0.0 , square.y, 0.0 , 0.0 } + test;
    float sqrt_a = sqrtf(test.x);

    return CalcSomeVal(sqrt_a, vec);
}

inline Vec4 TransformValues(const Vec4& r9, const Vec4& rcx, const Vec4& r8, const Vec4& rdx) {

    /* todo: verify order of values after completion */
    Vec4 square_row_0 = r8 * r8;
    Vec4 sum_rows = square_row_0 + Vec4{ square_row_0.y, r8.x, r8.x, square_row_0.x };
    Vec4 sum_rows2 = sum_rows + Vec4{ sum_rows.x, square_row_0.x, square_row_0.x, sum_rows.w };

    Vec4 test = r8 * Vec4{ 1,-1,-1,-1 };
    test = test / sum_rows2.z;

    Vec4 test2 = Vec4{ rcx.x, rcx.w, rcx.z, rcx.y } * test.x;
    test2 = test2 * Vec4{ -1,1,-1,1 };

    Vec4 test3 = Vec4{ rcx.z, rcx.y, rcx.x, rcx.w } * test.z;
    test3 = test3 * Vec4{ -1,-1,1,1 };

    Vec4 test4 = Vec4{ rcx.y, rcx.z, rcx.w, rcx.x } * test.y; //
    test4 = test4 * Vec4{ -1, 1,1,-1 };

    Vec4 test5 = rcx * test.w;
    Vec4 sumtest1 = test4 + test3;
    test5 = test5 + test2;
    sumtest1 = sumtest1 + test5;

    float w = ValueShift(&sumtest1.w);
    float w2 = ValueShift2(&w);
    float divRes = std::isnan(w / w2) ? 1.0 : (w/w2);

    Vec4 result_top = Vec4{ 0, sumtest1.x, sumtest1.y, sumtest1.z } * divRes;

    //////////////////////////////////// Next Row //////////////////////////////////////

    Vec4 later0 = r8 * Vec4{ 1, -1,-1,-1 };
    Vec4 laterxmm = later0 / 1.0 /* this 1 might be wrong */;

    Vec4 lTest0 = Vec4{ r9.x, r9.w, r9.z, r9.y } *laterxmm.x;
    lTest0 = lTest0 * Vec4{ -1,1,-1,1 };

    Vec4 ltest3 = Vec4{ r9.z, r9.y, r9.x, r9.w } *laterxmm.z;
    ltest3 = ltest3 * Vec4{ -1,-1,1,1 };

    Vec4 ltest4 = Vec4{ r9.y, r9.z, r9.w, r9.x } *laterxmm.y;
    ltest4 = ltest4 * Vec4{ -1, 1,1,-1 };

    Vec4 testv0 = r9 * laterxmm.w;
    Vec4 sum = (ltest3 + ltest4) + (testv0 + lTest0);

    float rw_0 = ValueShift(&sum.w);
    float rw_1 = ValueShift2(&rw_0);
    float divRes2 = std::isnan(rw_0 / rw_1) ? 1.0 : (rw_0 / rw_1);
    Vec4 result_bottom = Vec4{ 0, sum.x, sum.y, sum.z } * divRes2;

    //////////////////////////////////// continue this //////////////////////////////////////

    Vec4 total_result = result_top + result_bottom;
    total_result = total_result * -0.25;

    Vec4 flat_a = FlattenStreamVector(total_result);

    Vec4 xmm5 = r8;
    Vec4 res_3 = Vec4{ flat_a.x, flat_a.w, flat_a.z, flat_a.y } *r8.x;
    res_3 = res_3 * Vec4{ -1,1,-1,1 };
    Vec4 res_2 = Vec4{ flat_a.z, flat_a.y, flat_a.x, flat_a.w } *r8.z;
    res_2 = res_2 * Vec4{ -1,-1,1,1 };
    Vec4 res_1 = Vec4{ flat_a.y, flat_a.z, flat_a.w, flat_a.x } *r8.y;
    res_1 = res_1 * Vec4{ -1,1,1,-1 };

    Vec4 res_0 = flat_a * r8.w;
    res_0 = res_0 + res_3;
    res_2 = res_2 + res_1;
    res_3 = res_0 + res_2;

    return res_3;
}

inline void MutateMatrices(Matrix4x4* mat) {

    /* Checks sign value of row's z value*/
    Vec4 val_r_3 = mat->row0 * mat->row2;
    Vec4 r3_sum = val_r_3 + Vec4{ val_r_3.y, mat->row2.x, mat->row2.x, val_r_3.x };
    val_r_3 = r3_sum + Vec4{ r3_sum.x, val_r_3.z, val_r_3.z, r3_sum.w };
    if (val_r_3.z < 0) { mat->row0 = 0.0 - mat->row0; }


    Vec4 val_r_2 = mat->row0 * mat->row1;
    Vec4 r2_sum = val_r_2 + Vec4{ val_r_2.y, mat->row0.z, mat->row0.z, val_r_2.z };
    val_r_2 = r2_sum + Vec4{ r2_sum.x, val_r_2.z, val_r_2.z, r2_sum.w };
    if (val_r_3.z < 0) { mat->row1 = 0.0 - mat->row1; }


    Vec4 val_r_1 = mat->row1 * mat->row3;
    Vec4 r1_sum = val_r_1 + Vec4{ val_r_1.y, mat->row1.x, mat->row1.x, val_r_1.x };
    val_r_1 = r1_sum + Vec4{ r1_sum.x, val_r_1.x, val_r_1.x, r1_sum.w };
    if (val_r_1.z < 0) { mat->row3 = 0.0 - mat->row3; }

    Vec4 newRow = TransformValues(mat->row1, mat->row2, mat->row0, mat->row2);
    Vec4 newRow2 = TransformValues(mat->row3, mat->row0, mat->row1, mat->row0); /* Check this for nan */

    *mat = Matrix4x4{ mat->row0, mat->row1, newRow, newRow2 };

}


