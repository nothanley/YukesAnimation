/* Creates and maintains objects for Yukes motion tracks*/
#pragma once
#include "../DataIO/BinaryIO.h"
#include "../Codec/C_YukesDecoder.h"

class YukesAnimRegistry;
class YukesAnim {

public:
	uint32_t m_BoneHash = 0;

	YukesAnim(YukesAnimRegistry* parentRegistry) {
		this->m_Registry = parentRegistry;
		CreateMotionTrack();
	}

private:
	YukesAnimRegistry* m_Registry;
	std::istream* stream;

	void CreateMotionTrack();
	friend YukesDecoder;
};