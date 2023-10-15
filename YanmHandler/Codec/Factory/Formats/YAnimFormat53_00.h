/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
#include "../../../RotationHelper.h"

using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat53_00 : public YAnimFormat {

public:
    void Decode() override {
        // printf("\nDecoding 0x53 format...");

        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 4; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream(); }

        fs->seekg(streamPos);
        GetAnimOrigin(fs,&m_Track->m_StreamDelta);

        RotationHelper debug(this->vec_a, this->vec_b);
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;
    std::vector<TranslateKey> vec_a;
    std::vector<TranslateKey> vec_b;

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);

        switch (streamIndex) {
        case 0x0:
            Get8bSignedByteArray(fs, &numSegments, &this->vec_a);
            break;
        case 0x1:
            Get8bSignedByteArray(fs, &numSegments, &this->vec_b);
            break;
        case 0x2:
            //DecodeEulerStreamS16(fs, &numSegments, &this->rotationsRow2 ); 
            break;
        case 0x3:
            DecodeTransStream16S(fs, &numSegments, &m_Track->m_Translations);
            break;    }
    }


};

