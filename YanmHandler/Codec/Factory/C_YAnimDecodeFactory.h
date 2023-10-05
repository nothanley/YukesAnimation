/* Declares factory class for polymorphic YANM stream decoders*/
#include <iostream>
#include <map>
#include <memory>

/* Enumerates all known encode signatures */
enum {
    Format0 = 0x2300,
    Format1 = 0x1,
    Format2 = 0x1,
    Format3 = 0x1,
    Format4 = 0x1,
    Format5 = 0x1,
};

/* Base decoder, morphs into known formats */
class YAnimFormat {
public:
    virtual ~YAnimFormat() {}
    virtual void Decode(std::istream& fs) = 0;
protected:
    std::istream* fs;
    float origin_x, origin_y, origin_z;
};

#include "YukesAnimEnum.h"
class DecoderFactory {
/* Takes a unique encode value, ties all decoder methods together
using a pointer map and finds appropriate match to polymorphed class*/
public:
    DecoderFactory() {
        decoders_[Format0] = std::make_unique<YAnimFormat00>();
        decoders_[Format1] = std::make_unique<YAnimFormat01>();
        decoders_[Format2] = std::make_unique<YAnimFormat02>();
        decoders_[Format3] = std::make_unique<YAnimFormat03>();
        decoders_[Format4] = std::make_unique<YAnimFormat04>();
        decoders_[Format5] = std::make_unique<YAnimFormat05>();
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

