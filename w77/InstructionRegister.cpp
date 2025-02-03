#include "InstructionRegister.h"

void InstructionRegister::load(uint8_t instr) {
    instruction = instr;  // Store the given instruction
}

uint8_t InstructionRegister::get() const {
    return instruction;  // Return the current instruction
}
