#include "../Animation/C_YukesAnim.h"
using namespace BinaryIO;

void YukesDecoder::DecodeMotionTrack() {
	std::istream* fs = this->m_SourceTrack->stream;
	this->m_EncodeFormat = ReadUShortBE(*fs);
	this->m_BitSize = ReadUShortBE(*fs);
	this->m_BoneHash = ReadUInt32BE(*fs);

	printf("\nEncode Type: %d", m_EncodeFormat);
	ReadTrackStream(fs);
}

void YukesDecoder::ReadTrackAttributes(std::istream* fs) {
	 
}

void YukesDecoder::ReadTrackStream(std::istream* fs) {
	DecoderFactory factory;
	decoder_ = factory.CreateDecoder(this->m_EncodeFormat);
	if (!decoder_) { return; }
	decoder_->Decode(*fs);
}