#ifndef PROGRAMCOUNTER_H
#define PROGRAMCOUNTER_H

#include <cstdint>

class ProgramCounter {
private:
    uint8_t pc{0};

public:
    void increment();
    void set(uint8_t address);
    uint8_t get() const;
};

#endif // PROGRAMCOUNTER_H
