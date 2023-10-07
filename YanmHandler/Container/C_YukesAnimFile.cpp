#include "../Container/C_YukesAnimFile.h"

using namespace std;
using namespace BinaryIO;

void YukesAnimFile::Load() {
	if (this->fs == nullptr)
		this->fs = new std::ifstream(this->filePath, ios::binary);
	if (!fs->good())
		throw("Cannot read YANM stream.");
	ValidateContainer();
	if (this->isOk) { ReadContents(); }
}

void YukesAnimFile::ReadContents() {
	printf("Opening File: %s\n", filePath.c_str());
	this->m_Registry = new YukesAnimRegistry(this);
}

void YukesAnimFile::ValidateContainer() {
	fs->seekg(0);
	uint32_t signature = ReadUInt32(*fs);
	this->fileSize = ReadUInt32(*fs);

	// Validates type and version
	if (ntohl(signature) == YANM)
		this->isOk = true;
}

static uint64_t GetFileBufferSize(std::filebuf* buffer) {
	buffer->pubseekoff(0, std::ios::end);
	std::streampos size = buffer->pubseekoff(0, std::ios::cur);
	return uint64_t(size);
}