// ProgramCounter.h
#ifndef PROGRAMCOUNTER_H
#define PROGRAMCOUNTER_H

#include <cstdint>

class ProgramCounter {
private:
    uint8_t pc{0};  // Initialize the counter to 0

public:
    void increment() { pc = (pc + 1) & 0xFF; }
    void set(uint8_t address) { pc = address; }
    uint8_t get() const { return pc; }
};

#endif // PROGRAMCOUNTER_H
