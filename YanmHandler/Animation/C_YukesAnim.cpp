#include "C_YukesAnim.h"
#include "../Container/C_YAnimRegistry.h"

void YukesAnim::CreateMotionTrack() {
	this->stream = m_Registry->fs;
	YukesDecoder motionTrack(this);
	motionTrack.DecodeMotionTrack();
}