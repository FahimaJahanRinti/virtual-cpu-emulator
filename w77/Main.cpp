#include <iostream>
#include <vector>
#include <string>
#include "CPU.h"
#include "Assembler.h"

int main() {
    // Example assembly program
    std::vector<std::string> assemblyCode = {
        "LOAD 0 15",   // Load 15 into R0
        "LOAD 1 8",    // Load 8 into R1
        "ADD 0 1",     // Add R1 to R0, store result in R0
        "WRITE 0",     // Output R0 (Display)
        "READ 1",      // Input from keyboard to R1
        "STORE 1 100", // Store R1 at memory address 100
        "NOP",          // No operation
        "BEQ 50",       // Branch if R0 == R1 to address 50
        "BNE 60",       // Branch if R0 != R1 to address 60
        "INT"           // Interrupt
    };

    // Create an assembler instance
    Assembler assembler;

    // Assemble the program
    std::vector<uint8_t> machineCode = assembler.assemble(assemblyCode);

    // Display machine code
    std::cout << "Machine code (Hexadecimal): ";
    for (auto byte : machineCode) std::cout << std::hex << (int)byte << " ";
    std::cout << std::endl;

    // Create a CPU instance and execute the program
    CPU cpu;
    cpu.execute(machineCode);

    // Display the final CPU state
    cpu.displayState();

    return 0;
}
