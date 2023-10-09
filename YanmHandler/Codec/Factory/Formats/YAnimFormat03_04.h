/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat03_04 : public YAnimFormat {

public:
    void Decode() override {
        //  printf("\nDecoding 0x304 format...");

        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 3; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream();  }

        fs->seekg(streamPos);
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        if (streamIndex == 0x1) { GetAnimOrigin(fs,&m_Track->m_StreamDelta); }
        this->streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);

        switch (streamIndex) {
        case 0x0:
            DecodeEulerStreamS16(fs, &numSegments, &m_Track->m_CustomTransforms);
            break;
        case 0x1:
            DecodeTransStream16S(fs,&numSegments,&m_Track->m_Translations);
            break;
        case 0x2:
            ReadRotationMatrices(fs, &numSegments, &m_Track->m_Rotations);
            break;   }
    }

};