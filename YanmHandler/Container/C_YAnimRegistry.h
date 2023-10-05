#pragma once
#include "../Animation/C_YukesAnim.h"

class YukesAnimFile;

class YukesAnimRegistry {
public:
	YukesAnimRegistry( YukesAnimFile* file ) {
		this->m_FileObj = file;
		InitializeStream();
	}

private:
	YukesAnimFile* m_FileObj;
	uint64_t m_size;
	uint16_t m_numTracks;
	std::vector<YukesAnim> tracks;
	std::ifstream* fs;

	void InitializeStream();
	void GetYanmAttributes();
	void ReadYANMStream();
	void CreateAnimationTrack();
	friend class YukesAnim;
};