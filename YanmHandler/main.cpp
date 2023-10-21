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
    //std::vector<std::string> paths = GetFilesInDirectory("C://Users//wauke//Desktop///14412.bpe");

    for (int i = 0; i < 20; i++)
        YukesActsFile anims("C://Users//wauke//Desktop//YukesAnim_Examples//Old_Gen_Ent//2k14_Entrance//@Entrance05.pac//EVZ//@548.pac//0000.dat", 1);

    YukesActsFile("C://Users//wauke//Desktop//YukesAnim_Examples//Old_Gen_Ent//2k14_Entrance//@Entrance05.pac//EVZ//@548.pac//0000.dat", 1);


    YukesActsFile("C://Users//wauke//Desktop//YukesAnim_Examples//Old_Gen_Ent//2k14_Entrance//@Entrance05.pac//EVZ//@548.pac//0000.dat", 1);

    YukesActsFile("C://Users//wauke//Desktop//YukesAnim_Examples//Old_Gen_Ent//2k14_Entrance//@Entrance05.pac//EVZ//@548.pac//0000.dat", 1);

    YukesActsFile("C://Users//wauke//Desktop//YukesAnim_Examples//Old_Gen_Ent//2k14_Entrance//@Entrance05.pac//EVZ//@548.pac//0000.dat", 1);

    YukesActsFile("C://Users//wauke//Desktop//YukesAnim_Examples//Old_Gen_Ent//2k14_Entrance//@Entrance05.pac//EVZ//@548.pac//0000.dat", 1);


    YukesActsFile("C://Users//wauke//Desktop//YukesAnim_Examples//Old_Gen_Ent//2k14_Entrance//@Entrance05.pac//EVZ//@548.pac//0000.dat", 1);

    //anims.m_Contents->GetNode(1);
    //anims.m_Contents->LoadAllNodes();

    //for (const auto& path : paths) {
        //printf("\nNewFile: %s", path.c_str());
        //YukesActsFile anims(path.c_str());
        //anims.m_Contents->LoadAllNodes();
        //uint32_t value = anims.m_Contents->numAnims;
        //printf("\nValue: %d", value);
    //}

   
    return 0;
}