/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
#include "../../../Animation/Utils/RotationHelper.h"

using namespace AnimUtils;
using namespace BinaryIO;


class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat02_04 : public YAnimFormat {

public:
    void Decode() override {
        // printf("\nDecoding 0x0204 format...");

        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 2; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream(); }

        fs->seekg(streamPos);

        RotationHelper debug(m_Track->m_Rotations, this->m_Track->m_BoneHash, 16);
        this->m_Track->m_Rotations = debug.unpackedTransforms;
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);

        if (streamIndex == 0x0) { 
            DecodeEulerStreamS16(fs, &numSegments, &m_Track->m_CustomTransforms); 
        }
        else {
            Get16bSignedMatrix(fs,&numSegments,&m_Track->m_Rotations);
        }
    }


};