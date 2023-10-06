/* Decodes unique bitstream */
#include "../../../AnimationUtils.h"
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat02_01 : public YAnimFormat {

public:
    void Decode(std::istream* stream) override {
        this->fs = stream;
        printf("\nDecoding 0x0201 format...");

        /* Holds a constant of 3 bitstreams pertaining
        to translation and rotation vectors */
        ReadStream();
        fs->seekg(streamPos);
        ReadIKStream();
        fs->seekg(streamPos);
        printf("\nMotion Runtime: %d frames\n", runtime);
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;
    uint8_t ikType = 0;
    Matrix3x4 ikOrigin;

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        this->streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);

        ReadTranslateStream(&numSegments);
    }

    void ReadIKStream() {
        uint8_t value = ReadByte(*fs);
        this->ikType = ReadByte(*fs);

        uint16_t numSegments = ReadUShortBE(*fs);
        uint32_t streamPointer = ReadUInt32BE(*fs);
        streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);

        this->ikOrigin = AnimUtils::GetIKOriginMatrix(fs);
        ParseIKBitstream(&numSegments);
    }

    void ReadTranslateStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Vec3 transform = { URotToDegree * ReadShortBE(*fs),
                URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };

            uint16_t numKeys = ReadUShortBE(*fs);
            this->runtime += numKeys;
            this->translations.push_back(TranslateKey{ transform, numKeys });    }
    }

    void ParseIKBitstream(uint16_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            uint8_t numKeys = ReadByte(*fs);
            Vec4 transform;

            switch (ikType) {
            case (0xD1):
                transform = AnimUtils::ShiftIKMatrix(fs, this->ikOrigin);
                break;
            case (0xC0):
                break; }

            this->ikTransforms.push_back( IKKey{transform,numKeys} );
        }
    }


  
};

