/* Validates YANM file and initializes a stream upon validation success  */
#pragma once
#include "../Container/C_YAnimRegistry.h"
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
	float m_Version;
	YukesAnimRegistry* m_Registry = nullptr;
	~YukesAnimFile() {}

	YukesAnimFile(const char* FilePath) {
		this->filePath = FilePath;
		Load();
		fs->close();
	}

	YukesAnimFile(std::ifstream* stream) {
		this->fs = stream;
		Load();
	}

private:
	std::ifstream* fs = nullptr;
	bool isOk = false;
	void Load();
	void ReadContents();
	void ValidateContainer();
	friend class YukesAnimRegistry;
};
