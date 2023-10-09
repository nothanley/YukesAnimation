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


template <typename KeyType>
void GetVectorRuntime(const std::vector<KeyType>& keys, unsigned int* runtime) {
	if (keys.size() == 0) { return; }
	*runtime = 0;
	for (const auto& key : keys)
		*runtime += key.duration;
}

void YukesAnim::GetTrackRuntime() {
	GetVectorRuntime(this->m_Translations,&runtime);
	GetVectorRuntime(this->m_Rotations, &runtime);
	GetVectorRuntime(this->m_IKTransforms, &runtime);
};

