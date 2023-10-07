/* Validates Animation bundle format and maintains all animation nodes */
#pragma once
#include <fstream>
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

	virtual ~YukesActsFile() {}
	YukesActsFile(const char* FilePath) {
		this->filePath = FilePath;
		LoadFile();
		fs->close();
	}

private:
	std::ifstream* fs;
	bool isOk = false;
	bool isLegacyFormat = true;
	void LoadFile();
	void ReadLegacy();
	void ReadModern();
	void ValidateContainer();
};

class YukesActs {
public:
	virtual ~YukesActs() {}
	virtual void LoadTree() = 0;

protected:
	std::istream* fs;
	YukesActsFile* m_FileObj;
};