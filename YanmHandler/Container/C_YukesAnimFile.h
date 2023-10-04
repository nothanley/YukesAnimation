#pragma once
#include "../Animation/C_YukesAnim.h"
#pragma comment(lib, "Ws2_32.lib")

using namespace std;
using namespace BinaryIO;

class YukesAnimFile{
	enum {
		YANM = 0x59414E4D
	};

public:
	std::string filePath;
	uint64_t fileSize;
	uint32_t m_Version;

	YukesAnimFile(const char* FilePath) {
		this->filePath = FilePath;
		LoadFile();
	}

	void LoadFile();

private:
	std::ifstream* fs;
	bool isOk = false;

	void ReadContents();
	void ValidateContainer();
	friend class YukesAnim;
};
