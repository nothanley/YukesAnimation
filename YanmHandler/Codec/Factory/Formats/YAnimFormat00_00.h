/* Decodes unique bitstream */
class YAnimFormat;

class YAnimFormat00_00 : public YAnimFormat {
public:
    /* This codec defines an empty motion. Therefore we set a zero transform*/
    void Decode(std::istream* fs) override {
        this->origin = Vec4{ 0,0,0,0 };
        this->translations.resize(0);
    }
};

