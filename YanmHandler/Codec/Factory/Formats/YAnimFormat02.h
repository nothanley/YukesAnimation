class YAnimFormat;

class YAnimFormat02 : public YAnimFormat {
public:
    void Decode(std::istream& fs) override {
        printf("\n\nClass A");
    }
};

