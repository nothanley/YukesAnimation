/* Interacts with Decoder Factory to decode a serialized yukes animation track*/
#pragma once
#include "../Animation/C_YukesAnim.h"
#include "Factory/C_YAnimDecodeFactory.h"

class YukesAnim;
class YukesDecoder{

public:
	uint16_t m_EncodeFormat = 0;
	uint16_t m_BitSize = 0;
	uint32_t m_BoneHash = 0;

	YukesDecoder(YukesAnim* track) {
		this->m_SourceTrack = track;
	}

	void DecodeMotionTrack();

private:
	YukesAnim* m_SourceTrack;
	std::unique_ptr<YAnimFormat> decoder_;
	//void ReadTrackAttributes(std::istream* fs);
	void ReadTrackStream(std::istream* fs);
};

