#include "Registers.h"
#include <iostream>
#include <bitset>

void Registers::write(int index, uint8_t value) {
    if (index >= 0 && index < 4)
        reg[index] = value;
    else
        std::cout << "Error: Invalid register index " << index << std::endl;
}

uint8_t Registers::read(int index) {
    if (index >= 0 && index < 4) return reg[index];
    std::cout << "Error: Invalid register index " << index << std::endl;
    return 0;
}

void Registers::display() {
    for (int i = 0; i < 4; ++i) {
        std::cout << "R" << i << ": " << (int)(reg[i]) << std::endl;
        std::cout << "R" << i << ": " << std::bitset<8>(reg[i]) << std::endl;
    }
}
