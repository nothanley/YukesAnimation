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

	uint64_t m_size;
	uint16_t m_numTracks;
	uint32_t m_runtime;
	std::vector<YukesAnim*> tracks;

private:
	std::ifstream* fs;
	YukesAnimFile* m_FileObj;

	void InitializeStream();
	void GetYanmAttributes();
	friend class YukesAnim;
};

struct YukesAnimNode {
	YukesAnimRegistry* animation;
	std::string name;
	uint32_t address;
	uint32_t size;
};
