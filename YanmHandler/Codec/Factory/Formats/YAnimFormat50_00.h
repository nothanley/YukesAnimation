/* Decodes unique bitstream */
#include "../../../AnimationUtils.h"
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat50_00 : public YAnimFormat {

public:

    void Decode(std::istream* stream) override {
        this->fs = stream;
        printf("\nDecoding 0x50 format...");

        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 2; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream();  }

        fs->seekg(streamPos);
        printf("\nMotion Runtime: %d frames\n", runtime);
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
            ReadOtherStream(&numSegments); /* Unknown */
            break;
        case 0x1:
            ReadKeyStream(&numSegments); /* Unknown */
            break;}
    }

    void ReadOtherStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Vec3 transform = { U8RotToDegree * ReadSignedByte(*fs),
                U8RotToDegree * ReadSignedByte(*fs), U8RotToDegree * ReadSignedByte(*fs) };

            uint8_t numKeys = ReadByte(*fs);
            this->runtime += numKeys;
            this->other.push_back(TranslateKey{ transform, numKeys });
        }
    }

    void ReadKeyStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Vec3 transform = { U8RotToDegree * ReadSignedByte(*fs),
                U8RotToDegree * ReadSignedByte(*fs), U8RotToDegree * ReadSignedByte(*fs) };

            uint8_t numKeys = ReadByte(*fs);
            this->other.push_back(TranslateKey{ transform, numKeys });
        }
    }


};

 