/* Decodes unique bitstream */
#include "../../../Animation/C_YukesAnim.h"
#include "../../../Animation/AnimationUtils.h"
using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat01_00 : public YAnimFormat {

public:
    void Decode() override {
        //printf("\nDecoding 0x0100 format...");
        ReadStream();

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

        DecodeTransStream16S(fs,&numSegments, &m_Track->m_Translations);
    }

};