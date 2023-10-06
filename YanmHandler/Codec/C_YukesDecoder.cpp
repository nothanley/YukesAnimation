#include "../Animation/C_YukesAnim.h"
using namespace BinaryIO;

void YukesDecoder::DecodeMotionTrack() {
	std::istream* fs = this->m_SourceTrack->stream;
	this->m_EncodeFormat = ReadUShortBE(*fs);
	this->m_BitSize = ReadUShortBE(*fs);
	this->m_BoneHash = ReadUInt32BE(*fs);
	printf("\n\nEncode Type: %#x", m_EncodeFormat);
	ReadTrackStream(fs);
}

void YukesDecoder::ReadTrackStream(std::istream* fs) {
	DecoderFactory factory;
	decoder_ = factory.CreateDecoder(this->m_EncodeFormat);
	if (!decoder_) {
		printf("\nUnsupported bitstream %d", this->m_EncodeFormat);
		return; }

	decoder_->Decode(fs);
}