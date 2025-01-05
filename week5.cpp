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
    {"AND", 0x06},
    {"OR", 0x07},
    {"XOR", 0x08}
};

// Map for expected operand count per instruction
unordered_map<string, int> instructionOperands = {
    {"LOAD", 2}, {"ADD", 2}, {"SUB", 2}, {"STORE", 2}, {"NOP", 0},
    {"AND", 2}, {"OR", 2}, {"XOR", 2}
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
    uint8_t and_op(uint8_t a, uint8_t b) { return a & b; }
    uint8_t or_op(uint8_t a, uint8_t b) { return a | b; }
    uint8_t xor_op(uint8_t a, uint8_t b) { return a ^ b; }
    uint8_t nop() { return 0; }
};

// Registers Class
class Registers {
private:
    uint8_t reg[4]{}; // Initialize registers to zero

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

// Memory Class
class Memory {
private:
    uint8_t mem[256]{};  // Simulate 256 bytes of memory

public:
    void write(uint8_t address, uint8_t value) {
        if (address < 256) {
            mem[address] = value;
        } else {
            cout << "Error: Invalid memory address " << (int)address << endl;
        }
    }

    uint8_t read(uint8_t address) {
        if (address < 256) {
            return mem[address];
        } else {
            cout << "Error: Invalid memory address " << (int)address << endl;
            return 0;
        }
    }

    void displayMemory() {
        for (int i = 0; i < 256; ++i) {
            if (mem[i] != 0) {
                cout << "Memory[" << i << "]: " << (int)mem[i] << endl;
            }
        }
    }
};

// CPU Class
class CPU {
private:
    ALU alu;
    Registers registers;
    ProgramCounter pc;
    InstructionRegister ir;
    Memory memory;

    void fetchInstruction(const vector<uint8_t>& machineCode) {
        if (pc.get() < machineCode.size()) {
            ir.load(machineCode[pc.get()]);
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
                    uint8_t regIndex = machineCode[pc.get() + 1];
                    uint8_t operand = machineCode[pc.get() + 2];
                    registers.write(regIndex, operand);
                    pc.increment();
                    pc.increment();
                    pc.increment();
                    break;
                }
                case 0x02: { // ADD
                    uint8_t regIndex = machineCode[pc.get() + 1];
                    uint8_t operand = machineCode[pc.get() + 2];
                    registers.write(regIndex, alu.add(registers.read(regIndex), operand));
                    pc.increment(); 
                    pc.increment(); 
                    pc.increment();
                    break;
                }
                case 0x03: { // SUB
                    uint8_t regIndex = machineCode[pc.get() + 1];
                    uint8_t operand = machineCode[pc.get() + 2];
                    registers.write(regIndex, alu.sub(registers.read(regIndex), operand));
                    pc.increment(); 
                    pc.increment(); 
                    pc.increment();
                    break;
                }
                case 0x04: { // STORE
                    uint8_t regIndex = machineCode[pc.get() + 1];
                    uint8_t memoryAddress = machineCode[pc.get() + 2];
                    memory.write(memoryAddress, registers.read(regIndex));
                    pc.increment(); 
                    pc.increment(); 
                    pc.increment();
                    break;
                }
                case 0x05: { // NOP
                    pc.increment();
                    break;
                }
                case 0x06: { // AND.
                    uint8_t regIndex = machineCode[pc.get() + 1];
                    uint8_t operand = machineCode[pc.get() + 2];
                    registers.write(regIndex, alu.and_op(registers.read(regIndex), operand));
                    pc.increment(); 
                    pc.increment(); 
                    pc.increment();
                    break;
                }
                case 0x07: { // OR
                    uint8_t regIndex = machineCode[pc.get() + 1];
                    uint8_t operand = machineCode[pc.get() + 2];
                    registers.write(regIndex, alu.or_op(registers.read(regIndex), operand));
                    pc.increment(); 
                    pc.increment(); 
                    pc.increment();
                    break;
                }
                case 0x08: { // XOR
                    uint8_t regIndex = machineCode[pc.get() + 1];
                    uint8_t operand = machineCode[pc.get() + 2];
                    uint8_t result = alu.xor_op(registers.read(regIndex), operand);  // XOR R0 with 4
                    registers.write(regIndex, result);  // Store result in R0
                    pc.increment(); 
                    pc.increment(); 
                    pc.increment();
                    break;
                }
                default:
                    cout << "Error: Invalid opcode " << (int)opcode << endl;
                    return;
            }
        }
    }

    void displayState() {
        registers.display();
        memory.displayMemory();
    }
};

int main() {
    vector<string> assembly = {
        "LOAD 0 15",   // Load 15 into R0
        "LOAD 1 8",    // Load 8 into R1
        "ADD 0 1",     // ADD 1 to R0, store result in R0
        "OR 1 2",      // OR R1 and 2, store result in R1
        "XOR 0 4",     // XOR R0 and 4, store result in R0
        "STORE 0 100", // Store R0 at memory address 100
        "NOP"          // No operation
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
    cpu.displayState();

    return 0;
}
