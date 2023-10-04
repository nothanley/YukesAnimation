#include <fstream>
#include <winsock.h>
#include <stdint.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma once
using namespace std;

class YukesAnim
{
	enum {
		YANM = 0x4D544C73
	};

public:
	std::string filePath;
	std::filebuf* fileBuffer = new std::filebuf();
	uint32_t fileSize;

	YukesAnim() { }

	YukesAnim(const char* FilePath) {
		LoadFile(FilePath);
	}

	void LoadFile(const char* FilePath) {
		//verifies if file is readable
		ifstream inFile(FilePath);

		if (inFile.good())
			fileBuffer->open(FilePath, ios::in | ios::binary);
		else
			throw("Cannot Open File.");

		//store filepath to object
		filePath = FilePath;
		printf("Opening File: %s\n", FilePath);
		validateMTLs();

		//store file size
		fileBuffer->pubseekoff(0, std::ios::end);
		std::streampos size = fileBuffer->pubseekoff(0, std::ios::cur);
		this->fileSize = size;

		//reset-seek
		fileBuffer->pubseekpos(ios_base::beg);

		//reads through container data
		processMain();
	}

	void processMain() {

	}


	void validateMTLs() { //this should be bool

		DWORD magicSig;
		std::iostream fs(fileBuffer);

		//seeks magic and validates
		fileBuffer->pubseekpos(ios_base::beg);
		fs.read((char*)&magicSig, sizeof(DWORD));

		//validate type and version
		if (ntohl(magicSig) != YANM) {
			throw("File is not a valid MTLs container.");
		}

		//cout << "Verified MTLs container." << endl;
	}


private:

};
