#include <fstream>
#include <winsock.h>
#include <stdint.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma once
using namespace std;

class YukesAnim
{
	enum {
		YANM = 0x59414E4D
	};

public:
	std::string filePath;
	std::filebuf* fileBuffer = new std::filebuf();
	uint32_t fileSize;

	YukesAnim() { }

	YukesAnim(const char* FilePath) {
		this->filePath = FilePath;
		LoadFile();
	}

	void LoadFile() {

		ifstream inFile(this->filePath);
		if (inFile.good())
			fileBuffer->open(filePath, ios::in | ios::binary);
		else
			throw("Cannot Open File.");

		// store file attributes
		printf("Opening File: %s\n", filePath.c_str());
		ValidateContainer();
		this->fileSize = GetFileBufferSize(fileBuffer);

		// perform read operation
		ReadContents();
	}

private:
	bool isOk = false;

	void ReadContents() {
		printf("\nValidated File");

	}

	uint64_t GetFileBufferSize(std::filebuf* buffer) {
		buffer->pubseekoff(0, std::ios::end);
		std::streampos size = buffer->pubseekoff(0, std::ios::cur);
		return uint64_t(size);
	}

	void ValidateContainer() {
		DWORD magicSig;
		std::iostream fs(fileBuffer);

		//seeks magic and validates
		fileBuffer->pubseekpos(ios_base::beg);
		fs.read((char*)&magicSig, sizeof(DWORD));

		//validate type and version
		if (ntohl(magicSig) != YANM)
			throw("File is not a valid MTLs container.");

		this->isOk = true;
	}


};
