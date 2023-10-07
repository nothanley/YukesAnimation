/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat53_00 : public YAnimFormat {

public:
    void Decode() override {
        printf("\nDecoding 0x53 format...");

        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 4; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream(); }

        fs->seekg(streamPos);
        GetAnimOrigin(fs,&m_Track->m_StreamDelta);
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);

        switch (streamIndex) {
        case 0x0:
            DecodeRotationStream8S(fs, &numSegments, &m_Track->m_Rotations);
            break;
        case 0x1:
            DecodeEulerStreamS8(fs, &numSegments, &m_Track->m_CustomTransforms);
            break;
        case 0x2:
            DecodeDeltaStreamS16(fs, &numSegments, &m_Track->m_CustomTransforms); /* Unknown */
            break;
        case 0x3:
            DecodeTransStream16S(fs, &numSegments, &m_Track->m_Translations);
            break;    }
    }

};

