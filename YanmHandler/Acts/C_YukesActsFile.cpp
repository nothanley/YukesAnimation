#pragma once
#include "C_YukesActsFile.h"
#include "../YukesActsLegacy.h"
using namespace std;
using namespace BinaryIO;


void YukesActsFile::LoadFile() {
	this->fs = new std::ifstream(this->filePath, ios::binary);
	if ( !fs->good() ) { throw("Cannot Open File."); }

	ValidateContainer();
	if (!this->isOk) { return; }
	if (isLegacyFormat) { ReadLegacy(); }
	else { ReadModern(); }
}

void YukesActsFile::ValidateContainer() {
	/* Validates type and version */
	fs->seekg(0);
	uint32_t signature = ReadUInt32(*fs);
	if (ntohl(signature) == AMYM) {
		this->isOk = true;
		this->isLegacyFormat = false;
		return;	}

	// Check for legacy format
	fs->seekg(0x24);
	signature = ReadUInt32(*fs);
	if (ntohl(signature) == ROOT) {
		this->isOk = true;
		this->isLegacyFormat = true;	}
}


void YukesActsFile::ReadLegacy() {
	this->m_Contents = new YukesActsLegacy(fs,this);
	m_Contents->LoadTree();
}

void YukesActsFile::ReadModern() {

}