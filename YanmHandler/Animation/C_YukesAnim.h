#pragma once
#include "../DataIO/BinaryIO.h"

class YukesAnimFile;
class YukesAnim {

public:
	YukesAnim( YukesAnimFile* file ) {
		this->m_FileObj = file;
		InitializeStream();
	}

private:
	YukesAnimFile* m_FileObj;
	uint64_t streamSize;
	std::ifstream* fs;

	void InitializeStream();
	void ReadYanmAttributes();
	void ReadYANMStream();
};