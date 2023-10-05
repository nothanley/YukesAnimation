#include "C_YukesAnim.h"
#include "../Container/C_YAnimRegistry.h"

void YukesAnim::GetAnimationTrack() {
	this->stream = m_Registry->fs;
	YukesDecoder motionTrack(this);
	motionTrack.DecodeMotionTrack();
}