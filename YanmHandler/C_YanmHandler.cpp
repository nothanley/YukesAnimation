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
	/* Replace path with custom path to debug */
    std::vector<std::string> paths = GetFilesInDirectory(
        "C:/Users/wauke/Desktop/YukesAnim_Examples/CMPunk_2015/@EVZ/131.bpe"    );
	
    /* Opens all files in directory*/
    for (auto& path : paths)
        YukesActsFile(path.c_str());

	return 0;
}