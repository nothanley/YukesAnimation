/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat50_00 : public YAnimFormat {

public:
    void Decode() override {
        //    printf("\nDecoding 0x50 format...");

        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 2; streamIndex++) {
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
        streamPos = fs->tellg();
        fs->seekg( uint64_t(streamPointer) + 0x8);

        switch (streamIndex) {
        case 0x0:
            DecodeEulerStreamS8(fs,&numSegments,&m_Track->m_CustomTransforms); /* Unknown */
            break;
        case 0x1:
            DecodeEulerStreamS8(fs, &numSegments, &m_Track->m_CustomTransforms); /* Unknown */
            break;}
    }


};

 