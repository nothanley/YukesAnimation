#include "../Animation/C_YukesAnim.h"
using namespace BinaryIO;

void YukesDecoder::DecodeMotionTrack() {
	std::istream* fs = this->m_SourceTrack->stream;

	this->m_EncodeFormat = ReadUShort(*fs);
	this->m_BitSize = ReadUShort(*fs);
	this->m_BoneHash = ReadUInt32(*fs);
	printf("\nEncode Type: %d", m_BoneHash);

	// From here on the motion track can be encoded and/or compressed in various ways. 
}

void YukesDecoder::ReadTrackAttributes(std::istream* fs) {
	 
}

void YukesDecoder::ReadTrackStream(std::istream* fs) {

}