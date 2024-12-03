#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cstdint>

using namespace std;

// Opcode map for assembly instructions
unordered_map<string, uint8_t> opcodeMap = {
    {"LOAD", 0x01},
    {"ADD", 0x02},
    {"SUB", 0x03},
    {"STORE", 0x04},
    {"NOP", 0x05}
};

// Map for expected operand count per instruction
unordered_map<string, int> instructionOperands = {
    {"LOAD", 2},  // Now takes a register index as operand
    {"ADD", 2},   // Now takes a register index as operand
    {"SUB", 2},   // Now takes a register index as operand
    {"STORE", 2}, // Now takes a register index as operand
    {"NOP", 0}
};

// Tokenize function to split each line of assembly code by spaces and ignore comments
vector<string> tokenize(const string& line) {
    vector<string> tokens;
    string trimmedLine = line.substr(0, line.find('#'));  // Ignore comments
    istringstream stream(trimmedLine);
    string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Assemble function to convert assembly code into machine code
vector<uint8_t> assemble(const vector<string>& assembly) {
    vector<uint8_t> machineCode;
    for (const auto& line : assembly) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue; // Skip empty lines

        // Check if instruction exists in the opcode map
        if (opcodeMap.find(tokens[0]) == opcodeMap.end()) {
            cout << "Error: Unknown instruction: " << tokens[0] << endl;
            continue;  // Skip this line
        }

        // Add opcode to machine code
        machineCode.push_back(opcodeMap[tokens[0]]);

        // Check for operands based on instruction
        if (tokens.size() > 1) {
            try {
                // The first operand is the register index or value
                machineCode.push_back(static_cast<uint8_t>(stoi(tokens[1])));

                // If there are two operands, the second is the value or register index
                if (tokens.size() > 2) {
                    machineCode.push_back(static_cast<uint8_t>(stoi(tokens[2])));
                }
            } catch (const invalid_argument&) {
                cout << "Error: Invalid operand for instruction: " << tokens[0] << endl;
            } catch (const out_of_range&) {
                cout << "Error: Operand value out of range for instruction: " << tokens[0] << endl;
            }
        }
    }
    return machineCode;
}

// ALU Class
class ALU {
public:
    uint8_t add(uint8_t a, uint8_t b) {
        return (a + b) & 0xFF; // 8-bit result
    }

    uint8_t sub(uint8_t a, uint8_t b) {
        return (a - b) & 0xFF; // 8-bit result
    }

    uint8_t nop() {
        return 0; // NOP does nothing
    }
};

// Registers Class
class Registers {
private:
    uint8_t reg[4]; // 4 general-purpose 8-bit registers
public:
    Registers() { fill(begin(reg), end(reg), 0); }

    void write(int index, uint8_t value) {
        if (index >= 0 && index < 4) {
            reg[index] = value;
        } else {
            cout << "Error: Invalid register index " << index << endl;
        }
    }

    uint8_t read(int index) {
        if (index >= 0 && index < 4) {
            return reg[index];
        } else {
            cout << "Error: Invalid register index " << index << endl;
            return 0;
        }
    }

    void display() {
        for (int i = 0; i < 4; ++i) {
            cout << "R" << i << ": " << (int)reg[i] << endl;
        }
    }
};

// Program Counter
class ProgramCounter {
private:
    uint8_t pc;
public:
    ProgramCounter() : pc(0) {}

    void increment() { 
        pc = (pc + 1) & 0xFF;
    }

    void set(uint8_t address) {
        pc = address;
    }

    uint8_t get() const { 
        return pc; 
    }
};

// Instruction Register
class InstructionRegister {
private:
    uint8_t instruction;
public:
    void load(uint8_t instr) {
        instruction = instr;
    }

    uint8_t get() const {
        return instruction;
    }
};

// CPU Class
class CPU {
private:
    ALU alu;
    Registers registers;
    ProgramCounter pc;
    InstructionRegister ir;

public:
    void execute(vector<uint8_t>& machineCode) {
        while (pc.get() < machineCode.size()) {
            ir.load(machineCode[pc.get()]);
            pc.increment();

            uint8_t opcode = ir.get();
            switch (opcode) {
                case 0x01: { // LOAD
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    registers.write(regIndex, operand); // Load to specified register
                    break;
                }
                case 0x02: { // ADD
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    uint8_t result = alu.add(registers.read(regIndex), operand);
                    registers.write(regIndex, result);
                    break;
                }
                case 0x03: { // SUB
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    uint8_t result = alu.sub(registers.read(regIndex), operand);
                    registers.write(regIndex, result);
                    break;
                }
                case 0x04: { // STORE
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t addr = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    cout << "STORE R" << (int)regIndex << " value: " << (int)registers.read(regIndex) << " at address " << (int)addr << endl;
                    break;
                }
                case 0x05: { // NOP
                    alu.nop();
                    break;
                }
                default:
                    cout << "Unknown opcode: " << (int)opcode << endl;
                    break;
            }
        }
    }

    void displayRegisters() {
        registers.display();
    }
};

// Main function
int main() {
    // Assembly code input
    vector<string> assembly = {
        "LOAD 0 5          # Load 5 into register R0",
        "ADD 0 3           # Add 3 to R0",
        "STORE 0 8         # Store result from R0 at memory address 8",
        "LOAD 1 10         # Load 10 into register R1",
        "SUB 1 2           # Subtract 2 from R1",
        "STORE 1 9         # Store result from R1 at memory address 9"
    };

    // Assemble into machine code
    vector<uint8_t> machineCode = assemble(assembly);

    // Display machine code
    cout << "Machine code: ";
    for (uint8_t byte : machineCode) {
        cout << hex << static_cast<int>(byte) << " ";
    }
    cout << endl;

    // Execute machine code
    CPU cpu;
    cout << "\nExecuting program...\n";
    cpu.execute(machineCode);

    // Display register state
    cout << "\nRegisters after execution:\n";
    cpu.displayRegisters();

    return 0;
}



