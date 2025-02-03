#ifndef INSTRUCTIONREGISTER_H
#define INSTRUCTIONREGISTER_H

#include <cstdint>

class InstructionRegister {
private:
    uint8_t instruction{0};  // Initialize instruction to zero

public:
    void load(uint8_t instr);  // Declaration: Store instruction
    uint8_t get() const;       // Declaration: Get current instruction
};

#endif // INSTRUCTIONREGISTER_H
