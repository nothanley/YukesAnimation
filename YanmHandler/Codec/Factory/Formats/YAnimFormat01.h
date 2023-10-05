class YAnimFormat;

class YAnimFormat01 : public YAnimFormat {
public:
    void Decode(std::istream& fs) override {
        printf("\n\nClass A");
    }
};

