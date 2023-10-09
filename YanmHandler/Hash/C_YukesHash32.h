/* Known hashing algos for Yukes formats */
#include <cstdint>

class YukesHash {

public:
	static uint32_t HashStringAs32Bit( const char* string ) {
		/* Hashes a string to a 32-bit value, most commonly used to ID bone joints */

		uint32_t result = 0x0;
		uint32_t val = 0x0;

		while ( *string != 0x0) {
			val = (result << 0x7) | *string;
			result = val ^ (result >> 0x19);
			string++;
		}

		return result;
	}

};