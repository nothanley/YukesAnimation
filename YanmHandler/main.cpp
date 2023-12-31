// YanmHandler.cpp : This file contains the 'main' function. Program execution begins and ends there.//
/* Utility mainly to debug reader class in CMD. Use this to test reader on acts/legacy file*/

#include "Acts/C_YukesActsFile.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
std::vector<std::string> GetFilesInDirectory(const std::string& path) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(path))
        if (entry.is_regular_file())
            files.push_back(entry.path().string());
    return files;
}

int main()
{
    std::string path = "C://Users//wauke//Desktop//0000.dat";
    YukesActsFile anims(path.c_str());

    anims.m_Contents->LoadAllNodes();
    uint32_t value = anims.m_Contents->numAnims;
    printf("\nValue: %d", value);
    return 0;
}