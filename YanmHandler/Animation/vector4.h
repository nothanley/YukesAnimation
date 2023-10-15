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
    Vec4 result = square * ((square * ((square * -2.38899e-008) + 2.75256e-006)) + -0.000198409);
    result = interpVec * ((square * ((square * (result + 0.00833333)) + -0.166667)) + 1.0);
    return result;
}


inline Vec4 FlattenMatrix(const Vec4* vec_a, const Vec4* vec_b, const float* interpolation) {

    Vec4 vec_mult = (*vec_a) * (*vec_b);
    Vec4 val_3 = Vec4{ vec_mult.y, vec_b->x, vec_b->x, vec_mult.x } + vec_mult;
    Vec4 val_4 = Vec4{ val_3.x, vec_mult.x, vec_mult.x, val_3.w } + val_3;
    Vec4 val_0 = sqrt(1 - (Vec4{ val_4.z, val_4.z, val_4.z, val_4.z } *val_4.z));
    if (std::isnan(val_0.x)) { val_0 = Vec4{ 0,0,0,0 }; }

    Vec4 normal = NormalizeVector(Vec4(val_0 / val_4.z));
    normal = normal * Vec4{ 0, (-*interpolation) + 1, *interpolation, 0 };
    Vec4 normInt = InterpolateVector(normal) / val_0;

    if (std::isnan(normInt.x)) {
        normInt = Vec4{ 0,(-*interpolation) + 1, *interpolation,0 };
    }

    return  (*vec_a * normInt.x) + (*vec_b * normInt.y);
}

inline Vec4 GetTopLevelVector(Matrix4x4* mat, const float interpolation) {
    float subInterp = (interpolation -
        (interpolation * interpolation)) * 2.0;

    Vec4 tfmTop = FlattenMatrix(&mat->row0, &mat->row1, &interpolation);
    Vec4 tfmBottom = FlattenMatrix(&mat->row2, &mat->row3, &interpolation);
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
    if (result.x == 0.0) { result = Vec4{ 1,1,1,1 }; }

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