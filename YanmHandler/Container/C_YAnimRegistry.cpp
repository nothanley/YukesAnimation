#include "C_YanimRegistry.h"
#include "../Container/C_YukesAnimFile.h"
#include <iostream>
using namespace std;

void YukesAnimRegistry::InitializeStream() {
	this->fs = m_FileObj->fs;
	ReadYANMStream();
}

void YukesAnimRegistry::ReadYANMStream() {
	GetYanmAttributes();

	// Collect all animation tracks
	for (int i = 0; i < this->m_numTracks; i++) {
		CreateAnimationTrack();
	}
}

void YukesAnimRegistry::GetYanmAttributes() {
	m_FileObj->m_Version = ReadUShort(*fs);
	this->m_numTracks = ReadUShort(*fs);
	uint32_t dataBegin = ReadUInt32(*fs);
	cout << "YANM Version: " << m_FileObj->m_Version;
}

void YukesAnimRegistry::CreateAnimationTrack() {

	YukesAnim animTrack(this);
	

}