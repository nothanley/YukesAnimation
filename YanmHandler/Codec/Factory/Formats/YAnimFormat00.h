class YAnimFormat;

class YAnimFormat00 : public YAnimFormat {
public:
    void Decode(std::istream& fs) override {
        printf("\n\nClass A");
    }
};
