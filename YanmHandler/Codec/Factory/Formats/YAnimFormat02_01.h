/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
#include "../../../Animation/Utils/RotationHelper.h"

using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat02_01 : public YAnimFormat {

public:
    void Decode() override {
        //printf("\nDecoding 0x0201 format...");
        ReadStream();
        InitializeIK();
        RotationHelper debug(this->vec_a, this->m_Track->m_BoneHash, 16);
        this->m_Track->m_Rotations = debug.unpackedTransforms;
    }

private:
    std::streampos streamPos;
    std::vector<TranslateKey> vec_a;
    int streamIndex = 0;
    uint8_t ikType = 0;
    Matrix3x4 ikOrigin;

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        this->streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);
        Get16bSignedByteArray(fs, &numSegments, &this->vec_a);
        fs->seekg(streamPos);
    }

    void InitializeIK() {
        uint8_t value = ReadByte(*fs);
        this->ikType = ReadByte(*fs);

        uint16_t numSegments = ReadUShortBE(*fs);
        uint32_t streamPointer = ReadUInt32BE(*fs);
        streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);

        this->ikOrigin = AnimUtils::GetIKOriginMatrix(fs);
        DecodeIKStream(fs,&numSegments,&ikType,&ikOrigin,&m_Track->m_IKTransforms);
        fs->seekg(streamPos);
    }

};

