/* Decodes unique bitstream */
#include "../../../AnimationUtils.h"
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat23_00 : public YAnimFormat {

public:

    void Decode(std::istream* stream) override {
        this->fs = stream;
        printf("\nDecoding 0x23 format...");

        /* Holds a constant of 3 bitstreams pertaining 
        to translation and rotation vectors */
        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 3; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream();  }

        GetAnimOrigin();
        printf("\nMotion Runtime: %d frames\n", runtime);
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;
    
    void GetAnimOrigin() {
        fs->seekg(streamPos);
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
            ReadTranslateStream(&numSegments);
            break;
        case 0x1:
            ReadRotationStream(&numSegments);
            break;
        case 0x2:
            ReadKeyStream(&numSegments);
            break;   }
    }

    void ReadTranslateStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Vec3 transform = { URotToDegree * ReadShortBE(*fs),
                URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };

            uint16_t numKeys = ReadUShortBE(*fs);
            this->runtime += numKeys;
            this->translations.push_back(TranslateKey{ transform, numKeys });
        }
    }

    void ReadRotationStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Matrix3x3 mat;
            mat.row0 = { URotToDegree * ReadShortBE(*fs),
                URotToDegree* ReadShortBE(*fs), URotToDegree* ReadShortBE(*fs) };

            uint16_t numKeys = ReadUShortBE(*fs);
            this->rotations.push_back(MatrixKey{ mat, numKeys });   }
    }

    void ReadKeyStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Vec3 transform = { URotToDegree * ReadShortBE(*fs),
                URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };

            uint16_t numKeys = ReadUShortBE(*fs);
            this->other.push_back(TranslateKey{ transform, numKeys });   }
    }


};

 