/* Creates and maintains objects for Yukes motion tracks*/
#pragma once
#include "AnimationUtils.h"
#include "../DataIO/BinaryIO.h"

class YukesAnimRegistry;
class YukesDecoder;
class YukesAnim {

public:
	unsigned int runtime = 0;
	uint16_t m_EncodeFormat = 0;
	uint16_t m_BitSize = 0;
	uint32_t m_BoneHash = 0;

	Vec4 origin{0,0,0,0};
	std::vector<TranslateKey> m_Translations;
	std::vector<MatrixKey> m_Rotations;
	std::vector<IKKey> m_IKTransforms;
	std::vector<TranslateKey> m_CustomTransforms;
	
	YukesAnim(YukesAnimRegistry* parentRegistry) {
		this->m_Registry = parentRegistry;
		CreateMotionTrack();
		GetTrackIntrinsics();
	}

private:
	YukesAnimRegistry* m_Registry;
	std::istream* stream;

	void CreateMotionTrack();
	void GetTrackIntrinsics();
	void GetTrackRuntime();
	friend YukesDecoder;
};