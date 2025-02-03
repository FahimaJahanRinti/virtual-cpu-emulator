#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

class Memory {
private:
    uint8_t mem[256]{};  // Simulate 256 bytes of memory

public:
    void write(uint8_t address, uint8_t value);
    uint8_t read(uint8_t address);
    void displayMemory();
};

#endif // MEMORY_H
