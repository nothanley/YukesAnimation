/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
#include "../../../Animation/RotationHelper.h"

using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat42_00 : public YAnimFormat {

public:
    void Decode() override {
        // printf("\nDecoding 0x42 format...");
        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 2; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream(); }

        fs->seekg(streamPos);

        RotationHelper debug(this->vec_a, std::vector<TranslateKey>{}, this->m_Track->m_BoneHash,8);
        this->m_Track->m_Rotations = debug.unpackedTransforms;
    }

private:
    std::streampos streamPos;
    std::vector<TranslateKey> vec_a;
    int streamIndex = 0;

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
            DecodeTransStream16S(fs, &numSegments, &m_Track->m_Translations);
            break; }
    }

};

