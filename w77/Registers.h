#ifndef REGISTERS_H
#define REGISTERS_H

#include <cstdint>

class Registers {
private:
    uint8_t reg[4]{}; // Initialize registers to zero

public:
    void write(int index, uint8_t value);
    uint8_t read(int index);
    void display();
};

#endif // REGISTERS_H
