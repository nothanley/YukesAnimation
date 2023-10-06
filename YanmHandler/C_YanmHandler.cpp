// YanmHandler.cpp : This file contains the 'main' function. Program execution begins and ends there.//
/* Utility mainly to debug reader class in CMD. Use this to test reader on a yanm file*/

#include "Container/C_YukesAnimFile.h"
#include <iostream>

int main()
{
	/* Replace path with custom file to debug */
	std::string filePath = "C:/Users/wauke/Desktop/YukesAnim_Examples/punk_sample_3.yanm";
	YukesAnimFile(filePath.c_str());
	return 0;
}