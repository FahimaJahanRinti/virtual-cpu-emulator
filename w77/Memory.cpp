#include "Memory.h"
#include <iostream>
#include <bitset>

void Memory::write(uint8_t address, uint8_t value) {
    if (address < 256) {
        mem[address] = value;
    } else {
        std::cout << "Error: Invalid memory address " << (int)address << std::endl;
    }
}

uint8_t Memory::read(uint8_t address) {
    if (address < 256) {
        return mem[address];
    } else {
        std::cout << "Error: Invalid memory address " << (int)address << std::endl;
        return 0;  // Return zero for invalid address access
    }
}

void Memory::displayMemory() {
    bool memoryDumped = false;

    // Display all memory locations where the value is non-zero
    for (int i = 0; i < 256; ++i) {
        if (mem[i] != 0) {
            if (!memoryDumped) {
                std::cout << "Displaying Non-Zero Memory Locations:" << std::endl;
                memoryDumped = true;
            }
            std::cout << "Memory[" << i << "]: " << (int)(mem[i])
                      << " (" << std::bitset<8>(mem[i]) << ")" << std::endl;
        }
    }

    if (!memoryDumped) {
        std::cout << "No non-zero memory locations to display." << std::endl;
    }
}
