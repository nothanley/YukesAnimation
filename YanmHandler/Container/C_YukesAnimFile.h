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
	bool doCollectRuntime = false;
	YukesAnimRegistry* m_Registry = nullptr;
	~YukesAnimFile() {}

	YukesAnimFile(const char* FilePath, bool collectRuntimes = false) {
		this->filePath = FilePath;
		this->doCollectRuntime = collectRuntimes;
		Load();
		fs->close();
	}

	YukesAnimFile(std::ifstream* stream, bool collectRuntimes = false) {
		this->fs = stream;
		this->doCollectRuntime = collectRuntimes;
		Load();
	}

	YukesAnimFile(std::ifstream* stream, const uint32_t& requestTrack, bool collectRuntimes=false) {
		this->fs = stream;
		this->doCollectRuntime = collectRuntimes;
		Load(requestTrack);
	}


private:
	std::ifstream* fs = nullptr;
	bool isOk = false;
	void Load(const uint32_t& searchTrack=0);
	void ReadContents(const uint32_t& searchTrack=0);
	void ValidateContainer();
	friend class YukesAnimRegistry;
};
