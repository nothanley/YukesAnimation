#include "DataIO/BinaryIO.h"
#include "../Animation/C_YukesAnim.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma once

using namespace std;
using namespace BinaryIO;

class YukesAnimFile{
	enum {
		YANM = 0x59414E4D
	};

public:
	std::string filePath;
	uint64_t fileSize;

	YukesAnimFile(const char* FilePath) {
		this->filePath = FilePath;
		LoadFile();
	}

	void LoadFile() {
		ifstream inFile(this->filePath);
		if (inFile.good())
			fileBuffer->open(filePath, ios::in | ios::binary);
		else { throw("Cannot Open File."); }

		ValidateContainer();
		if (this->isOk) { ReadContents(); }
	}

private:
	std::filebuf* fileBuffer = new std::filebuf();
	std::ifstream* fs;
	bool isOk = false;

	void ReadContents() {
		this->fileSize = GetFileBufferSize(fileBuffer);
		printf("Opening File: %s\n", filePath.c_str());
		YukesAnim(this->fs);
	}

	uint64_t GetFileBufferSize(std::filebuf* buffer) {
		buffer->pubseekoff(0, std::ios::end);
		std::streampos size = buffer->pubseekoff(0, std::ios::cur);
		return uint64_t(size);
	}

	void ValidateContainer() {
		//seeks magic and validates
		this->fs = new std::ifstream(fileBuffer);
		fileBuffer->pubseekpos(ios_base::beg);
		uint32_t signature = ReadUInt32(*fs);

		//validate type and version
		if (ntohl(signature) == YANM)
			this->isOk = true;
	}


};
