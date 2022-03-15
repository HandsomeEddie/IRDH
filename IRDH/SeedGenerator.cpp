#include "SeedGenerator.h"



void SeedGenerator::RandomBinary() {
	uint8_t start = 0x12;
	uint8_t a = start;
	for (int i = 0; ; ++i) {
		int newbit = (((a >> 6) ^ (a >> 5)) & 1);
		a = ((a << 1) | newbit) & 0x7f;
		std::cout << std::bitset<8>(a) << std::endl;
		if (a == start) {
			std::cout << "repetition period is " << std::to_string(i) << std::endl;
			break;
		}
	}
}


