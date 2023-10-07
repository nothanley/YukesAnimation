/* Maintains a registry of all animation tracks within a given YANM stream*/
#pragma once
#include "../Animation/C_YukesAnim.h"

class YukesAnimFile;

class YukesAnimRegistry {
public:
	virtual ~YukesAnimRegistry() {
		for (auto& track : tracks)
			delete track; }

	YukesAnimRegistry( YukesAnimFile* file ) {
		this->m_FileObj = file;
		InitializeStream();
	}

private:
	YukesAnimFile* m_FileObj;
	uint64_t m_size;
	uint16_t m_numTracks;
	std::vector<YukesAnim*> tracks;
	std::ifstream* fs;

	void InitializeStream();
	void GetYanmAttributes();
	friend class YukesAnim;
};

struct YukesAnimNode {
	YukesAnimRegistry* animation;
	std::string name;
};
