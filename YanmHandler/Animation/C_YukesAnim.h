#include "../DataIO/BinaryIO.h"
using namespace std;

class YukesAnim {

public:
	YukesAnim( std::ifstream* stream ) {
		this->fs = stream;
		ReadYANMStream();
	}

private:
	uint64_t streamSize;
	std::ifstream* fs;

	void ReadYanmAttributes() {
		uint32_t magic = ReadUInt32(*fs);
		this->streamSize = ReadUInt32(*fs);
	}

	void ReadYANMStream() {
		ReadYanmAttributes();

	}

};