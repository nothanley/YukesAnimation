#include "C_YanimRegistry.h"
#include "../Container/C_YukesAnimFile.h"
#include <iostream>
using namespace std;

static float ReadFileVersion(std::istream* fs) {
	unsigned short value = ReadUShortBE(*fs);
	float floatValue = value >> 0x8;
	return floatValue;
}

void YukesAnimRegistry::InitializeStream() {
	this->fs = m_FileObj->fs;
	this->getRuntimes = this->m_FileObj->doCollectRuntime;
	GetYanmAttributes();
	for (int i = 0; i < this->m_numTracks; i++) {
		// Collect all animation tracks
		YukesAnim* anim = new YukesAnim(this);
		this->tracks.push_back(anim);
		if (anim->runtime > this->m_runtime) { this->m_runtime = anim->runtime; }
	}
	//printf("\nAll tracks loaded.\n");
}

void YukesAnimRegistry::GetYanmAttributes() {
	m_FileObj->m_Version = ReadFileVersion(fs);
	this->m_numTracks = ReadUShortBE(*fs);
	uint32_t streamPointer = ReadUInt32BE(*fs);

	if (m_FileObj->m_Version != 5.0 && m_FileObj->m_Version != 4.0) {
		/* 5.0 spans through more modern iterations, 4.0 spans through PS4 & PS3 gen*/
		throw("Unsupported YANM format");}

	//printf("\nYANM version %.1f", m_FileObj->m_Version );
}

