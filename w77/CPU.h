#ifndef CPU_H
#define CPU_H

#include "ALU.h"
#include "Registers.h"
#include "ProgramCounter.h"
#include "InstructionRegister.h"
#include "Memory.h"
#include "InputDevice.h"
#include "OutputDevice.h"
#include <vector>
#include <stack>

class CPU {
private:
    ALU alu;
    Registers registers;
    ProgramCounter pc;
    InstructionRegister ir;
    Memory memory;
    InputDevice inputDevice;
    OutputDevice outputDevice;
    std::stack<uint8_t> callStack;  // Stack for subroutine calls

    void fetchInstruction(const std::vector<uint8_t>& machineCode);

public:
    void execute(const std::vector<uint8_t>& machineCode);
    void displayState();
};

#endif // CPU_H
