/* Decodes unique bitstream */
#include "../../../Animation/AnimationUtils.h"
using namespace AnimUtils;
using namespace BinaryIO;

class YAnimFormat; /* Forward declare parent type*/
class YAnimFormat53_00 : public YAnimFormat {

public:
    void Decode() override {
        // printf("\nDecoding 0x53 format...");

        this->streamPos = fs->tellg();
        for (streamIndex; streamIndex < 4; streamIndex++) {
            fs->seekg(streamPos);
            ReadStream(); }

        fs->seekg(streamPos);
        MergeRotations();
        GetAnimOrigin(fs,&m_Track->m_StreamDelta);
    }

private:
    std::streampos streamPos;
    int streamIndex = 0;
    std::vector<TranslateKey> rotationsRow0;
    std::vector<TranslateKey> rotationsRow1;
    std::vector<TranslateKey> rotationsRow2;

    void MergeRotations() {

        int rowRun = 0;
        for (const auto& key : this->rotationsRow1) {
            rowRun += key.duration;
        }

        GetVectorRuntime(m_Track->m_Translations, &m_Track->runtime);
        std::vector<MatrixKey>* vec = &m_Track->m_Rotations;

        //debug
        for (const auto& key : this->rotationsRow1) {
            MatrixKey mat;
            mat.transform.row0 = key.transform;
            mat.duration = key.duration;
            vec->push_back(mat);
        }
    }


    void ReadStream() {
        uint32_t streamPointer = ReadUInt32BE(*fs);
        uint32_t numSegments = ReadUInt32BE(*fs);
        streamPos = fs->tellg();
        fs->seekg(uint64_t(streamPointer) + 0x8);
        
        /* Todo: decode stream 1 */

        switch (streamIndex) {
        case 0x0:
            DecodeRotationStream8S(fs, &numSegments, &m_Track->m_Rotations);
            break;
        case 0x1:
            DebugDecodeEulerStreamS8(fs, &numSegments, &this->rotationsRow1 );

            break;
        case 0x2:
            //DecodeEulerStreamS16(fs, &numSegments, &this->rotationsRow2 ); 
            break;
        case 0x3:
            DecodeTransStream16S(fs, &numSegments, &m_Track->m_Translations);
            break;    }
    }

    template <typename KeyType>
    void GetVectorRuntime(const std::vector<KeyType>& keys, unsigned int* runtime) {
        if (keys.size() == 0) { return; }
        *runtime = 0;
        for (const auto& key : keys)
            *runtime += key.duration;
    }

};

