#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cstdint>
#include <bitset>

using namespace std;

// Opcode map for assembly instructions
unordered_map<string, uint8_t> opcodeMap = {
    {"LOAD", 0x01},
    {"ADD", 0x02},
    {"SUB", 0x03},
    {"STORE", 0x04},
    {"NOP", 0x05},
    {"AND", 0x06},  // New logical operation
    {"OR", 0x07},   // New logical operation
    {"XOR", 0x08}   // New logical operation
};

// Map for expected operand count per instruction
unordered_map<string, int> instructionOperands = {
    {"LOAD", 2}, {"ADD", 2}, {"SUB", 2}, {"STORE", 2}, {"NOP", 0},
    {"AND", 2}, {"OR", 2}, {"XOR", 2}  // New logical operations
};

// Tokenize function
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

// Assemble function
vector<uint8_t> assemble(const vector<string>& assembly) {
    vector<uint8_t> machineCode;
    for (const auto& line : assembly) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        if (opcodeMap.find(tokens[0]) == opcodeMap.end()) {
            cout << "Error: Unknown instruction: " << tokens[0] << endl;
            continue;
        }

        machineCode.push_back(opcodeMap[tokens[0]]);

        int expectedOperands = instructionOperands[tokens[0]];
        if (tokens.size() - 1 != expectedOperands) {
            cout << "Error: Incorrect number of operands for " << tokens[0] << endl;
            continue;
        }

        try {
            for (size_t i = 1; i < tokens.size(); ++i) {
                machineCode.push_back(static_cast<uint8_t>(stoi(tokens[i])));
            }
        } catch (...) {
            cout << "Error: Invalid operand in instruction: " << tokens[0] << endl;
        }
    }
    return machineCode;
}

// ALU Class
class ALU {
public:
    uint8_t add(uint8_t a, uint8_t b) { return (a + b) & 0xFF; }
    uint8_t sub(uint8_t a, uint8_t b) { return (a - b) & 0xFF; }
    uint8_t nop() { return 0; }
    
    // Logical operations
    uint8_t andOp(uint8_t a, uint8_t b) { return a & b; }
    uint8_t orOp(uint8_t a, uint8_t b) { return a | b; }
    uint8_t xorOp(uint8_t a, uint8_t b) { return a ^ b; }
};

// Registers Class
class Registers {
private:
    uint8_t reg[4]{};

public:
    void write(int index, uint8_t value) {
        if (index >= 0 && index < 4)
            reg[index] = value;
        else
            cout << "Error: Invalid register index " << index << endl;
    }

    uint8_t read(int index) {
        if (index >= 0 && index < 4) return reg[index];
        cout << "Error: Invalid register index " << index << endl;
        return 0;
    }

    void display() {
        for (int i = 0; i < 4; ++i) {
            cout << "R" << i << ": " << (int)reg[i] << endl;
        }
    }
};

// Program Counter Class
class ProgramCounter {
private:
    uint8_t pc{0};

public:
    void increment() { pc = (pc + 1) & 0xFF; }
    void set(uint8_t address) { pc = address; }
    uint8_t get() const { return pc; }
};

// Instruction Register Class
class InstructionRegister {
private:
    uint8_t instruction{0};

public:
    void load(uint8_t instr) { instruction = instr; }
    uint8_t get() const { return instruction; }
};

// CPU Class
class CPU {
private:
    ALU alu;
    Registers registers;
    ProgramCounter pc;
    InstructionRegister ir;

    void fetchInstruction(const vector<uint8_t>& machineCode) {
        if (pc.get() < machineCode.size()) {
            ir.load(machineCode[pc.get()]);
            pc.increment();
        } else {
            cout << "Error: Program counter out of bounds!" << endl;
        }
    }

public:
    void execute(const vector<uint8_t>& machineCode) {
        while (pc.get() < machineCode.size()) {
            fetchInstruction(machineCode);
            uint8_t opcode = ir.get();

            switch (opcode) {
                case 0x01: { // LOAD
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    registers.write(regIndex, operand);
                    break;
                }
                case 0x02: { // ADD
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    registers.write(regIndex, alu.add(registers.read(regIndex), operand));
                    break;
                }
                case 0x03: { // SUB
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    registers.write(regIndex, alu.sub(registers.read(regIndex), operand));
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
                case 0x05: // NOP
                    alu.nop();
                    break;
                case 0x06: { // AND
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    registers.write(regIndex, alu.andOp(registers.read(regIndex), operand));
                    break;
                }
                case 0x07: { // OR
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    registers.write(regIndex, alu.orOp(registers.read(regIndex), operand));
                    break;
                }
                case 0x08: { // XOR
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    pc.increment();
                    pc.increment();
                    registers.write(regIndex, alu.xorOp(registers.read(regIndex), operand));
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

// Main Function
int main() {
    vector<string> assembly = {
        "LOAD 0 15         # Load 15 (0b1111) into R0",
        "LOAD 1 8          # Load 8 (0b1000) into R1",
        "AND 0 1           # Perform R0 AND R1",
        "OR 0 1            # Perform R0 OR R1",
        "XOR 0 1           # Perform R0 XOR R1",
        "NOP               # No operation",
    };

    vector<uint8_t> machineCode = assemble(assembly);

    cout << "Machine code: ";
    for (uint8_t byte : machineCode)
        cout << hex << (int)byte << " ";
    cout << endl;

    cout << "Machine code (Decimal): ";
    for (uint8_t byte : machineCode)
        cout << dec << (int)byte << " ";
    cout << endl;

    cout << "Machine code (Binary): ";
    for (uint8_t byte : machineCode)
        cout << bitset<8>(byte) << " ";
    cout << endl;

    CPU cpu;
    cpu.execute(machineCode);

    cout << "\nRegisters after execution:\n";
    cpu.displayRegisters();

    return 0;
}
