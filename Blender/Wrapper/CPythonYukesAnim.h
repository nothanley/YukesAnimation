#include <cstring>
#include "..\..\YanmHandler\Acts\C_YukesActsFile.h"
#pragma once

extern "C" __declspec(dllexport) const uint64_t * GetBranchData(const char* path, uint32_t* size)
{
    std::vector<uint64_t> branchValues;

    // Read branches within file
    YukesActsFile yukesFile(path);
    yukesFile.m_Contents->LoadAllNodes();
    *size = yukesFile.m_Contents->nodes.size();  /* Update python size pointer */

    // Populate Branch Vector
    for (const auto& node : yukesFile.m_Contents->nodes) {
        uint64_t keyframeCount = node.animation->m_runtime;
        uint64_t trackCount = node.animation->m_numTracks;
        branchValues.push_back(keyframeCount);
        branchValues.push_back(trackCount); 
    }

    uint64_t* result = new uint64_t[branchValues.size()];
    std::copy(branchValues.begin(), branchValues.end(), result);
    return result;
}

extern "C" __declspec(dllexport) void GetBranchNames(const char* path, char*** strings)
{
    // Read branches within file
    YukesActsFile yukesFile(path);
    yukesFile.m_Contents->LoadAllNodes();

    std::vector<std::string> stringVec;
    *strings = new char* [stringVec.size()];

    // Populate Branch Vector
    for (const auto& node : yukesFile.m_Contents->nodes) {
        stringVec.push_back(node.name);  }

    // Allocate memory for an array of C-style strings
    unsigned int numAnims = yukesFile.m_Contents->numAnims;

    *strings = new char* [numAnims];

    // Copy the strings from the vector to the array
    for (int i = 0; i < numAnims; i++) {
        const char* str = stringVec[i].c_str();
        size_t len = strlen(str) + 1; // +1 for null-terminator
        (*strings)[i] = new char[len];
        strcpy_s((*strings)[i], len, str);
    }
}

extern "C" __declspec(dllexport) const uint32_t* GetNodeBoneHashes(const char* path, int index, /* Stores Vector Size*/ int* size)
{
    // Read branches within file
    YukesActsFile yukesFile(path);
    YukesAnimNode* node = yukesFile.m_Contents->GetNode(index);
    std::vector<uint32_t> vec;

    // Populate Branch Vector
    for (const auto& track : node->animation->tracks) {
        vec.push_back(track->m_BoneHash);   }

    *size = static_cast<int>(vec.size());
    uint32_t* result = new uint32_t[vec.size()];
    std::copy(vec.begin(), vec.end(), result);
    return result;
}


extern "C" __declspec(dllexport) const float* GetTranslationKeysForBone(const char* Path, uint32_t motionHash, int index,
    /* Stores (Dynamically) Vector Size*/ int* size)
{
    std::vector<float> transforms;

    // Read branches within file
    YukesActsFile yukesFile(Path, index);
    YukesAnimNode* node = &yukesFile.m_Contents->nodes[index];

    // Find Anim Key
    for (const auto& track : node->animation->tracks) {
        if (track->m_BoneHash == motionHash) {
            if (track->hasIK)
                for (const auto& motion : track->m_IKTransforms) {
                    Vec3 rotation_vector{
                            motion.transform.x, motion.transform.y, motion.transform.z };
                    transforms.push_back(rotation_vector.x);
                    transforms.push_back(rotation_vector.y);
                    transforms.push_back(rotation_vector.z);
                    transforms.push_back(motion.duration);     }
            else
                for (const auto& motion : track->m_Translations) {
                    Vec3 rotation_vector{
                            motion.transform.x, motion.transform.y, motion.transform.z };
                    transforms.push_back(rotation_vector.x);
                    transforms.push_back(rotation_vector.y);
                    transforms.push_back(rotation_vector.z);
                    transforms.push_back(motion.duration);  }
            break;     }
    }

    //Return All Branch Identities
    *size = static_cast<int>(transforms.size());
    float* result = new float[transforms.size()];
    std::copy(transforms.begin(), transforms.end(), result);

    return result;
}



extern "C" __declspec(dllexport) const float* GetRotationKeysForBone(const char* Path, uint32_t motionHash, int index,
    /* Stores (Dynamically) Vector Size*/ int* size)
{
    std::vector<float> transforms;

    // Read branches within file
    YukesActsFile yukesFile(Path, index);
    YukesAnimNode* node = &yukesFile.m_Contents->nodes[index];

    // Find Anim Key
    for (const auto& track : node->animation->tracks) {
        if (track->m_BoneHash == motionHash) {
            for (const auto& motion : track->m_Rotations) {
                Vec3 rotation_vector{
                        motion.transform.row0.x, motion.transform.row0.y, motion.transform.row0.z };
                transforms.push_back(rotation_vector.x);
                transforms.push_back(rotation_vector.y);
                transforms.push_back(rotation_vector.z);
                transforms.push_back(motion.duration);}   
            break;  }
    }

    //Return All Branch Identities
    *size = static_cast<int>(transforms.size());
    float* result = new float[transforms.size()];
    std::copy(transforms.begin(), transforms.end(), result);

    return result;
}

// Free allocated mem
extern "C" __declspec(dllexport) void FreeUInt32Vector(uint32_t * vector)
{
    delete[] vector;
}


// Free allocated mem
extern "C" __declspec(dllexport) void FreeUInt64Vector(uint64_t * vector)
{
    delete[] vector;
}


// Free allocated mem
extern "C" __declspec(dllexport) void FreeFloatVector(float* vector)
{
    delete[] vector;
}

