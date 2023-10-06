/* Declares factory class for polymorphic YANM stream decoders*/
#include <iostream>
#include <map>
#include <memory>
#include "../../AnimationUtils.h"

/* Base decoder, morphs into known formats */
class YAnimFormat {
public:
    virtual ~YAnimFormat() {}
    virtual void Decode(std::istream* fs) = 0;
protected:
    std::istream* fs;
    std::vector<TranslateKey> translations;
    std::vector<MatrixKey> rotations;
    std::vector<TranslateKey> other;
    unsigned int runtime = 0;
    Vec4 origin; // matrix translate
};

#include "YukesAnimEnum.h"
class DecoderFactory {
/* Takes a unique encode value, ties all decoder methods together
using a pointer map and finds appropriate match to polymorphed class*/
public:
    DecoderFactory() {
        decoders_[0x0000] = std::make_unique<YAnimFormat00_00>();
        decoders_[0x0204] = std::make_unique<YAnimFormat02_04>();
        decoders_[0x0304] = std::make_unique<YAnimFormat03_04>();
        decoders_[0x2200] = std::make_unique<YAnimFormat22_00>();
        decoders_[0x2300] = std::make_unique<YAnimFormat23_00>();
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

