/* Decodes unique bitstream */
#include "../../../AnimationUtils.h"
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat52_00 : public YAnimFormat {

public:

    void Decode(std::istream* stream) override {
        this->fs = stream;
        printf("\nDecoding 0x52 format...");

        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 3; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream();  }

        fs->seekg(streamPos);
        GetAnimOrigin();
        printf("\nMotion Runtime: %d frames\n", runtime);
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;
    
    void GetAnimOrigin() {
        this->origin = Vec4{ ReadFloatBE(*fs), ReadFloatBE(*fs),
            ReadFloatBE(*fs), ReadFloatBE(*fs) };
    }

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        streamPos = fs->tellg();
        fs->seekg( uint64_t(streamPointer) + 0x8);

        switch (streamIndex) {
        case 0x0:
            ReadRotationStream(&numSegments);
            break;
        case 0x1:
            ReadKeyStream(&numSegments);
            break;
        case 0x2:
            ReadTranslateStream(&numSegments);
            break;   }
    }

    void ReadTranslateStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Vec3 transform = { ReadShortBE(*fs),
                ReadShortBE(*fs), ReadShortBE(*fs) };

            uint16_t numKeys = ReadUShortBE(*fs);
            this->runtime += numKeys;
            this->translations.push_back(TranslateKey{ transform, numKeys });
        }
    }

    void ReadRotationStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Matrix3x3 mat;
            mat.row0 = { U8RotToDegree * ReadSignedByte(*fs),
                U8RotToDegree* ReadSignedByte(*fs), U8RotToDegree* ReadSignedByte(*fs) };

            uint16_t numKeys = ReadByte(*fs);
            this->rotations.push_back(MatrixKey{ mat, numKeys });
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

 