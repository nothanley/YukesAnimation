#pragma once
#include "Animation/AnimationUtils.h"
#include <cmath>
#define _USE_MATH_DEFINES

const float M_PI = 3.14159265358979323846;
const float Rad2Deg = 180 / M_PI;

class QuatEulerConv
{

public:
    static Vec3 QuaternionToEuler(Vec4 q)
    {
        Vec3 angles;

        // roll (x-axis rotation)
        double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
        double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
        angles.x = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
        double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
        angles.y = 2 * std::atan2(sinp, cosp) - M_PI / 2;

        // yaw (z-axis rotation)
        double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
        double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
        angles.z = std::atan2(siny_cosp, cosy_cosp);

        angles = angles * Rad2Deg;
        return angles;
    }


};

