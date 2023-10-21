/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat40_00 : public YAnimFormat {

public:
    void Decode() override {
        ReadStream();
        fs->seekg(streamPos);
        RotationHelper debug(this->vec_a, this->m_Track->m_BoneHash, 8);
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
        fs->seekg( uint64_t(streamPointer) + 0x8);
        Get8bSignedByteArray(fs, &numSegments, &this->vec_a);
    }


};

 