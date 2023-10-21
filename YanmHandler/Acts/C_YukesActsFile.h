/* Validates animation tree format and maintains all animation nodes */
#pragma once
#include <fstream>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

class YukesActs;
class YukesActsFile {
	enum {
		AMYM = 0x414D594D,
		ROOT = 0x726F6F74
	};

public:
	std::string filePath;
	uint64_t fileSize;
	YukesActs* m_Contents;
	bool doCollectRuntime = false;

	virtual ~YukesActsFile() {}
	YukesActsFile(const char* FilePath, int index = -1) {
		this->filePath = FilePath;
		this->requestIndex = index;
		this->doCollectRuntime = true;
		LoadFile();
		fs->close();
	}

	YukesActsFile( const char* FilePath, const uint32_t& motionHash, int nodeIndex = -1, bool collectRuntime = false ) {
		this->filePath = FilePath;
		this->requestMotion = motionHash;
		this->requestIndex = nodeIndex;
		this->doCollectRuntime = collectRuntime;
		LoadFile();
		fs->close();
	}

	void close() { this->fs->close(); };

private:
	std::ifstream* fs;
	bool isOk = false;
	bool isLegacyFormat = true;
	uint32_t requestMotion = 0;
	int requestIndex = -1;
	void LoadFile();
	void ReadLegacy();
	void ReadModern();
	void ValidateContainer();
};

#include "../Container/C_YAnimRegistry.h"
class YukesActs {
public:
	virtual ~YukesActs() {}
	virtual void LoadTree() = 0;
	virtual void LoadAllNodes() = 0;
	virtual YukesAnimNode* GetNode(int index) { return nullptr; }
	virtual YukesAnimNode* GetOnlyNodeMotion(int index, const uint32_t& motionHash) { return nullptr; }
	uint32_t numAnims = 0;
	std::string name = "";
	std::vector<YukesAnimNode> nodes;

protected:
	std::istream* fs;
	YukesActsFile* m_FileObj;
};