#include "C_YukesDecoder.h"
using namespace BinaryIO;

void YukesDecoder::DecodeMotionTrack() {
	std::istream* fs = this->m_SourceTrack->stream;
	m_SourceTrack->m_EncodeFormat = ReadUShortBE(*fs);
	m_SourceTrack->m_BitSize = ReadUShortBE(*fs);
	m_SourceTrack->m_BoneHash = ReadUInt32BE(*fs);
	uint64_t streamEnd = uint64_t(fs->tellg()) + m_SourceTrack->m_BitSize - 0x8;

	//printf("\nEncode Type: %#x", m_SourceTrack->m_EncodeFormat);
	ReadTrackStream(fs);
	fs->seekg(streamEnd);
}

void YukesDecoder::ReadTrackStream(std::istream* fs) {
	DecoderFactory factory;
	decoder_ = factory.CreateDecoder(m_SourceTrack->m_EncodeFormat);
	if (!decoder_) {
		printf("\nUnsupported bitstream %#x", m_SourceTrack->m_EncodeFormat);
		return; }

	decoder_->Initialize(fs, this->m_SourceTrack);
	decoder_->Decode();
}