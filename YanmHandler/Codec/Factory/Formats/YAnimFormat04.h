class YAnimFormat;

class YAnimFormat04 : public YAnimFormat {
public:
    void Decode(std::istream* fs) override {
        printf("\n\nClass A");
    }
};
