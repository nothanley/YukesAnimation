#pragma once
#include "../AnimationUtils.h"
#include <cmath>
#define _USE_MATH_DEFINES

const float M_PI = 3.14159265358979323846;
const float Rad2Deg = 180 / M_PI;

class QuatEulerConv
{

public:
    static Vec3 QuaternionToEuler(Vec4 q)
    {
        Vec3 euler;

        // Normalize quaternion
        float length = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        q.x /= length;
        q.y /= length;
        q.z /= length;
        q.w /= length;

        // ZYX order
        float sinPitchCosYaw = 2.0f * (q.w * q.x + q.y * q.z);
        float cosPitchCosYaw = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);

        euler.x = std::atan2(sinPitchCosYaw, cosPitchCosYaw);

        float sinPitchSinYaw = 2.0f * (q.w * q.y - q.z * q.x);

        // Handle the special case when pitch is close to 90 degrees
        if (std::abs(sinPitchSinYaw) >= 1.0f) {
            euler.y = std::copysign(M_PI / 2.0f, sinPitchSinYaw); }
        else {
            euler.y = std::asin(sinPitchSinYaw);    }

        float sinRoll = 2.0f * (q.w * q.z + q.x * q.y);
        float cosRoll = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
        euler.z = std::atan2(sinRoll, cosRoll);

        // Convert angles from radians to degrees if needed
        euler.x = euler.x * (180.0f / M_PI);
        euler.y = euler.y * (180.0f / M_PI);
        euler.z = euler.z * (180.0f / M_PI);

        // Wrap the angles to the range [0, 360] degrees
        //euler.x = fmod(euler.x, 360.0f);
        //euler.y = fmod(euler.y, 360.0f);
        //euler.z = fmod(euler.z, 360.0f);

        //if (euler.x < 0.0f) euler.x += 360.0f;
        //if (euler.y < 0.0f) euler.y += 360.0f;
        //if (euler.z < 0.0f) euler.z += 360.0f;

        return euler;
    }


};

