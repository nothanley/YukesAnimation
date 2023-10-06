/* Decodes unique bitstream */
#include "../../../AnimationUtils.h"
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat02_04 : public YAnimFormat {

public:
    void Decode(std::istream* stream) override {
        this->fs = stream;
        printf("\nDecoding 0x0204 format...");

        /* Holds a constant of 3 bitstreams pertaining
        to translation and rotation vectors */
        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 2; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream(); }

        fs->seekg(streamPos);
        printf("\nMotion Runtime: %d keys\n", runtime);
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);

        if (streamIndex == 0x0) { ReadTranslateStream(&numSegments); }
        else { ReadMatrixStream(&numSegments); }
    }

    void ReadTranslateStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Vec3 transform = { URotToDegree * ReadShortBE(*fs),
                URotToDegree* ReadShortBE(*fs), URotToDegree* ReadShortBE(*fs) };

            uint16_t numKeys = ReadUShortBE(*fs);
            this->runtime += numKeys;
            this->translations.push_back(TranslateKey{ transform, numKeys });   }
    }

    void ReadMatrixStream(uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Matrix3x3 mat;
            uint16_t numKeys = ReadUShortBE(*fs);
            AnimUtils::StreamMatrix3x3(fs,&mat);
            this->rotations.push_back(MatrixKey{ mat, numKeys });   }
    }

};