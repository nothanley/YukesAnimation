/* Interacts with Decoder Factory to decode a serialized yukes animation track*/
#pragma once
#include "../Animation/C_YukesAnim.h"
#include "Factory/C_YAnimDecodeFactory.h"

class YukesAnim;
class YukesDecoder{

public:
	YukesDecoder(YukesAnim* track) {
		this->m_SourceTrack = track;
	}

	void DecodeMotionTrack();

private:
	YukesAnim* m_SourceTrack;
	std::unique_ptr<YAnimFormat> decoder_;
	void ReadTrackStream(std::istream* fs);
};

