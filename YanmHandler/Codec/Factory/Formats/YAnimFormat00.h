#include "../../../AnimationUtils.h"
//#include "../../../DataIO/BinaryIO.h"
using namespace BinaryIO;
const float FACTOR = 0.005493247883;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat00 : public YAnimFormat {


struct MotionSegment {
    Vec3 translation;
    uint16_t duration;
};

public:
    std::vector<MotionSegment> segments;

    void Decode(std::istream& stream) override {
        this->fs = &stream;
        printf("\nDecoding 0x23 format...");
        ReadStream();
    }

    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);

        /* Holds a constant of 3 bitstreams */
        for (int i = 0; i < 3; i++)
            ParseSubStream(&i, &numSegments);
        
    }

    void ParseSubStream(int* index, uint32_t* numSegments) {
        for (int k = 0; k < *numSegments; k++) {
            Vec3 translate = { FACTOR * ReadFloatBE(*fs),
                FACTOR * ReadFloatBE(*fs), FACTOR * ReadFloatBE(*fs) };

            uint16_t numKeys = ReadUShortBE(*fs);
            if (index == 0) { translate = Vec3{ 0,0,0 }; }
            this->segments.push_back( MotionSegment{ translate, numKeys } );
        }
    }

};

 