#include "C_YukesAnim.h"
#include "../Codec/C_YukesDecoder.h"
#include "../Container/C_YAnimRegistry.h"

void YukesAnim::CreateMotionTrack() {
	this->stream = m_Registry->fs;
	YukesDecoder motionTrack(this);
	motionTrack.DecodeMotionTrack();
}

void YukesAnim::GetTrackIntrinsics() {
	ApplyVectorDeltas( &this->m_Translations, &this->m_StreamDelta);
	GetTrackRuntime();
	//printf("\nMotion Runtime: %d frames\n", this->runtime);
}


void YukesAnim::GetTrackRuntime() {
	AnimUtils::GetVectorRuntime(this->m_Translations,&runtime);
	AnimUtils::GetVectorRuntime(this->m_Rotations, &runtime);
	AnimUtils::GetVectorRuntime(this->m_IKTransforms, &runtime);
};

