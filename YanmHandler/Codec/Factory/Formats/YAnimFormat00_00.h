/* Decodes unique bitstream */
#pragma once

class YAnimFormat;
class YAnimFormat00_00 : public YAnimFormat {
public:
    /* This codec defines an empty motion. Therefore we set a zero transform*/
    void Decode() override {
        this->m_Track->m_StreamDelta = Vec4{ 0,0,0,0 };
        this->m_Track->m_Translations.resize(0);
    }
};

