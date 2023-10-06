/* Holds collection of helper methods for performing io operations on binary streams, primarily file streams*/
#include <fstream>
#include <windows.h> 
#include <winsock.h>
#include <istream>
#include <vector>
#include <string>
using namespace std;

namespace BinaryIO {
		
	template <class T>
	void endswap(T* objp)
	{
		unsigned char* memp = reinterpret_cast<unsigned char*>(objp);
		std::reverse(memp, memp + sizeof(T));
	}

	// Big Endian
	static int ReadIntBE(istream& fs) {
		int value = 0;
		fs.read((char*)&value, 4);
		endswap(&value);
		return value;
	}
	static unsigned int ReadUIntBE(istream& in) {
		unsigned int value = 0;
		in.read((char*)&value, 4);
		endswap(&value);
		return value;
	}
	static uint32_t ReadUInt32BE(istream& in) {
		uint32_t value = 0;
		in.read((char*)&value, 4);
		endswap(&value);
		return value;
	}
	static int8_t ReadSInt8BE(istream& in) {
		int8_t value = 0;
		in.read((char*)&value, 1);
		endswap(&value);
		return value;
	}
	static int16_t ReadSInt16BE(istream& in) {
		int16_t value = 0;
		in.read((char*)&value, 2);
		endswap(&value);
		return value;
	}
	static int32_t ReadSInt32BE(istream& in) {
		int32_t value = 0;
		in.read((char*)&value, 4);
		endswap(&value);
		return value;
	}
	static uint64_t ReadUInt64BE(istream& in) {
		uint64_t value = 0;
		in.read((char*)&value, sizeof(uint64_t));
		endswap(&value);
		return value;
	}
	static unsigned short ReadUShortBE(istream& in) {
		unsigned short value = 0;
		in.read((char*)&value, 2);
		endswap(&value);
		return value;
	}
	static signed short ReadShortBE(istream& in) {
		signed short value = 0;
		in.read((char*)&value, 2);
		endswap(&value);
		return value;
	}
	static uint8_t ReadByteBE(istream& in) {
		uint8_t value = 0;
		in.read((char*)&value, 1);
		endswap(&value);
		return value;
	}
	static float ReadFloatBE(istream& in) {
		float value = 0;
		in.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
		endswap(&value);
		return value;
	}

	// Little Endian
	static int ReadInt(istream& fs) {
		int value = 0;
		fs.read((char*)&value, 4);
		return value;
	}
	static unsigned int ReadUInt(istream& in) {
		unsigned int value = 0;
		in.read((char*)&value, 4);
		return value;
	}
	static uint32_t ReadUInt32(istream& in) {
		uint32_t value = 0;
		in.read((char*)&value, 4);
		return value;
	}
	static int8_t ReadSInt8(istream& in) {
		int8_t value = 0;
		in.read((char*)&value, 1);
		return value;
	}
	static int16_t ReadSInt16(istream& in) {
		int16_t value = 0;
		in.read((char*)&value, 2);
		return value;
	}
	static int32_t ReadSInt32(istream& in) {
		int32_t value = 0;
		in.read((char*)&value, 4);
		return value;
	}
	static uint64_t ReadUInt64(istream& in) {
		uint64_t value = 0;
        in.read((char*)&value, sizeof(uint64_t));
		return value;
	}
	static unsigned short ReadUShort(istream& in) {
		unsigned short value = 0;
		in.read((char*)&value, 2);
		return value;
	}
	static short ReadShort(istream& in) {
		short value = 0;
		in.read((char*)&value, 2);
		return value;
	}
	static uint8_t ReadByte(istream& in) {
		uint8_t value = 0;
		in.read((char*)&value, 1);
		return value;
	}
	static float ReadFloat(istream& in) {
		float value = 0;
		in.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
		return value;
	}
	static bool ReadBool(istream& in) {
		return (ReadByte(in) != 0);
	}


    //Write-Methods
    static void WriteUInt64(ofstream* fs, uint64_t value) {
        fs->write(reinterpret_cast<char*>(&value), sizeof(uint64_t));
    }
    static void WriteUInt32(ofstream* fs, uint32_t value) {
        fs->write(reinterpret_cast<char*>(&value), sizeof(uint32_t));
    }
    static void WriteUInt16(ofstream* fs, uint16_t value) {
        fs->write(reinterpret_cast<char*>(&value), sizeof(uint16_t));
    }
    static void WriteByte(ofstream* fs, uint8_t value) {
        fs->write(reinterpret_cast<char*>(&value), sizeof(uint8_t));
    }
    static void WriteBool(ofstream* fs, bool flag) {
        fs->write(reinterpret_cast<char*>(&flag), sizeof(bool));
    }
    static void WriteInt32(ofstream* fs, int32_t value) {
        fs->write(reinterpret_cast<char*>(&value), sizeof(int32_t));
    }
    static void WriteFloat(ofstream* fs, float value) {
        fs->write(reinterpret_cast<char*>(&value), sizeof(float));
    }
    static void WriteString(ofstream* fs, std::string string){
        fs->write(string.c_str(),string.size()+1);
    }
    static void WriteChars(ofstream* fs, std::string value) {
        WriteUInt32(fs,value.size()+1);
        fs->write(value.c_str(),value.size()+1);
    }
    static void WriteSignature(ofstream* fs, std::string value) {
        uint32_t* streamHeader = reinterpret_cast<uint32_t*>(&value);
        *streamHeader = ntohl(*streamHeader);
        WriteUInt32(fs,*streamHeader);
    }
	static std::string ReadString(istream& fs, int size) {
		std::string value;
		value.resize(size);
		fs.read(&value[0], size);
		size_t nullPos = value.find('\0'); /* Remove trailing null characters */
		if (nullPos != std::string::npos)
			value.resize(nullPos);
		return value;	}
}


/*std::istream& operator>>(std::istream& input, int& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(int));
	return input;
}

std::istream& operator>>(std::istream& input, unsigned int& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(unsigned int));
	return input;
}

std::istream& operator>>(std::istream& input, int64_t& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(int64_t));
	return input;
}

std::istream& operator>>(std::istream& input, uint32_t& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
	return input;
}

std::istream& operator>>(std::istream& input, uint16_t& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(uint16_t));
	return input;
}

std::istream& operator>>(std::istream& input, uint8_t& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
	return input;
}

std::istream& operator>>(std::istream& input, uint64_t& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(uint64_t));
	return input;
}

std::istream& operator>>(std::istream& input, int32_t& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(int32_t));
	return input;
}


std::istream& operator>>(std::istream& input, int16_t& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(int16_t));
	return input;
}


std::istream& operator>>(std::istream& input, int8_t& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(int8_t));
	return input;
}

std::istream& operator>>(std::istream& input, float& value) {
	input.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
	return input;
}*/