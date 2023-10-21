/* Maintains a registry of all animation tracks within a given YANM stream*/
#pragma once
#include "../Animation/C_YukesAnim.h"

class YukesAnimFile;

class YukesAnimRegistry {
public:
	virtual ~YukesAnimRegistry() { }

	YukesAnimRegistry( YukesAnimFile* file, const uint32_t& requestTrack=0 ) {
		this->m_FileObj = file;
		this->SearchTrack = requestTrack;
		InitializeStream();
	}

	bool getRuntimes = false;
	uint64_t m_size;
	uint16_t m_numTracks;
	uint32_t m_runtime;
	uint32_t SearchTrack;
	std::vector<YukesAnim*> tracks;
	YukesAnimFile* m_FileObj;

private:
	std::ifstream* fs;
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
