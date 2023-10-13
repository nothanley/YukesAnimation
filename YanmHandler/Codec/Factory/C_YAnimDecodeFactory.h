/* Declares factory class for polymorphic YANM stream decoders*/
#include <iostream>
#include <map>
#include <memory>
#include <istream>
#include "../../Animation/AnimationUtils.h"

/* Base decoder, morphs into known formats */
class YukesAnim;
class YAnimFormat {
public:
    virtual ~YAnimFormat() {}
    virtual void Decode() = 0;
    void Initialize(std::istream* stream, YukesAnim* parent) {
        this->m_Track = parent;
        this->fs = stream;  }
protected:
    std::istream* fs;
    YukesAnim* m_Track;
};

#include "YukesAnimEnum.h"
class DecoderFactory {
/* Takes a unique encode value, ties all decoder methods together
using a pointer map and finds appropriate match to polymorphed class*/
public:
    DecoderFactory() {
        decoders_[0x0000] = std::make_unique<YAnimFormat00_00>();
        decoders_[0x0001] = std::make_unique<YAnimFormat00_01>();
        decoders_[0x0004] = std::make_unique<YAnimFormat00_04>();
        decoders_[0x0100] = std::make_unique<YAnimFormat01_00>();
        decoders_[0x0104] = std::make_unique<YAnimFormat01_04>();
        decoders_[0x0200] = std::make_unique<YAnimFormat02_00>();
        decoders_[0x0201] = std::make_unique<YAnimFormat02_01>();
        decoders_[0x0202] = std::make_unique<YAnimFormat02_02>();
        decoders_[0x0204] = std::make_unique<YAnimFormat02_04>();
        decoders_[0x0304] = std::make_unique<YAnimFormat03_04>();
        decoders_[0x2100] = std::make_unique<YAnimFormat21_00>();
        decoders_[0x2200] = std::make_unique<YAnimFormat22_00>();
        decoders_[0x2300] = std::make_unique<YAnimFormat23_00>();
        decoders_[0x4000] = std::make_unique<YAnimFormat40_00>();
        decoders_[0x4100] = std::make_unique<YAnimFormat41_00>();
        decoders_[0x4200] = std::make_unique<YAnimFormat42_00>();
        decoders_[0x4300] = std::make_unique<YAnimFormat43_00>();
        decoders_[0x5000] = std::make_unique<YAnimFormat50_00>();
        decoders_[0x5100] = std::make_unique<YAnimFormat51_00>();
        decoders_[0x5200] = std::make_unique<YAnimFormat52_00>();
        decoders_[0x5300] = std::make_unique<YAnimFormat53_00>();
        decoders_[0xA000] = std::make_unique<YAnimFormatA0_00>();
        decoders_[0xA200] = std::make_unique<YAnimFormatA2_00>();
    }

    // Traverses map for encode format, returns null for unsupported type
    std::unique_ptr<YAnimFormat> CreateDecoder(int encodeValue) {
        auto it = decoders_.find(encodeValue);
        if (it != decoders_.end())
            return std::move(it->second);
        else { return nullptr; }
    }

private:
    std::map<int, std::unique_ptr<YAnimFormat>> decoders_;
};

