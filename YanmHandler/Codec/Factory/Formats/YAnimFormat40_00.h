/* Decodes unique bitstream */
#include "../../../AnimationUtils.h"
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat40_00 : public YAnimFormat {

public:

    void Decode(std::istream* stream) override {
        this->fs = stream;
        printf("\nDecoding 0x40 format...");

        ReadStream();
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
        ReadRotationStream(&numSegments);
    }

    void ReadRotationStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Matrix3x3 mat;
            mat.row0 = { U8RotToDegree * ReadSignedByte(*fs),
                U8RotToDegree * ReadSignedByte(*fs), U8RotToDegree * ReadSignedByte(*fs) };

            uint16_t numKeys = ReadByte(*fs);
            this->runtime += numKeys;
            this->rotations.push_back(MatrixKey{ mat, numKeys });
        }
    }


};

 