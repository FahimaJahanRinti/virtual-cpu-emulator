#include <iostream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <string>
#include <sstream>
#include <cstdint>
#include <stack>
#include <chrono>

using namespace std;
using namespace chrono;

// Tokenize helper function
vector<string> tokenize(const string& line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Opcode map and operand count
unordered_map<string, uint8_t> opcodeMap = {
    {"LOAD", 0x01}, {"ADD", 0x02}, {"SUB", 0x03},
    {"STORE", 0x04}, {"NOP", 0x00}, {"AND", 0x06},
    {"OR", 0x07}, {"XOR", 0x08}, {"READ", 0x09},
    {"WRITE", 0x0A}, {"JMP", 0x0B}, {"CALL", 0x0C},
    {"RET", 0x0D}, {"BEQ", 0x0E}, {"BNE", 0x0F},
    {"INT", 0x10}
};

unordered_map<string, int> instructionOperands = {
    {"LOAD", 2}, {"ADD", 2}, {"SUB", 2}, {"STORE", 2}, {"NOP", 0},
    {"AND", 2}, {"OR", 2}, {"XOR", 2}, {"READ", 1}, {"WRITE", 1},
    {"JMP", 1}, {"CALL", 1}, {"RET", 0}, {"BEQ", 1}, {"BNE", 1}, {"INT", 0}
};

// Assemble function: Converts instructions into binary format
vector<bitset<8>> assembleBinary(const vector<string>& assembly) {
    vector<bitset<8>> machineCodeBinary;
    machineCodeBinary.reserve(assembly.size() * 3); // Pre-allocate memory for better performance
    for (const auto& line : assembly) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        if (opcodeMap.find(tokens[0]) == opcodeMap.end()) {
            cout << "Error: Unknown instruction: " << tokens[0] << endl;
            continue;
        }

        // Convert opcode to binary and add to machine code
        machineCodeBinary.push_back(bitset<8>(opcodeMap[tokens[0]]));
        int expectedOperands = instructionOperands[tokens[0]];

        if (tokens.size() - 1 != expectedOperands) {
            cout << "Error: Incorrect number of operands for " << tokens[0] << endl;
            continue;
        }

        try {
            for (size_t i = 1; i < tokens.size(); ++i) {
                machineCodeBinary.push_back(bitset<8>(stoi(tokens[i])));
            }
        } catch (...) {
            cout << "Error: Invalid operand in instruction: " << tokens[0] << endl;
        }
    }
    return machineCodeBinary;
}

// Memory class
class Memory {
private:
    bitset<8> mem[256];  // Simulate 256 bytes of memory in binary format

public:
    void write(uint8_t address, const bitset<8>& value) {
        if (address < 256) {
            mem[address] = value;
        } else {
            cout << "Error: Invalid memory address " << (int)address << endl;
        }
    }

    bitset<8> read(uint8_t address) {
        if (address < 256) {
            return mem[address];
        } else {
            cout << "Error: Invalid memory address " << (int)address << endl;
            return bitset<8>(0);
        }
    }

    void displayMemory() {
        for (int i = 0; i < 256; ++i) {
            if (mem[i].to_ulong() != 0) {  // Display only non-zero memory locations
                cout << "Memory[" << i << "]: " << mem[i] << endl;
            }
        }
    }
};

// Registers class
class Register {
private:
    bitset<8> value;  // Store value as binary

public:
    Register() : value(0) {}

    void load(const bitset<8>& val) { 
        value = val;
        }
    bitset<8> get() const { return value; }
    void increment() { value = value.to_ulong() + 1; }
    void decrement() { value = value.to_ulong() - 1; }
};

// CPU class
class CPU {
private:
    Register pc;   // Program counter (binary)
    Register ir;   // Instruction register (binary)
    Register reg[4];  // Four general-purpose registers (binary)
    Memory memory;
    stack<uint8_t> callStack;  // Stack for CALL and RET instructions

    void fetchInstruction(const vector<bitset<8>>& machineCode) {
        if (pc.get().to_ulong() < machineCode.size()) {
            ir.load(machineCode[pc.get().to_ulong()]);  // Fetch the instruction in binary
        } else {
            cout << "Error: Program counter out of bounds!" << endl;
        }
    }

public:
    void execute(const vector<bitset<8>>& machineCode) {
        auto start = high_resolution_clock::now(); // Start timer for performance profiling
        while (pc.get().to_ulong() < machineCode.size()) {
            fetchInstruction(machineCode);
            uint8_t opcode = ir.get().to_ulong();
            pc.increment();

            switch (opcode) {
    case 0x01: {  // LOAD
        uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        uint8_t value = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        reg[regNum].load(bitset<8>(value));
        break;
    }
    case 0x04: {  // STORE
        uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        memory.write(address, reg[regNum].get());
        break;
    }
    case 0x02: {  // ADD (Binary Addition)
        uint8_t regNum1 = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        uint8_t regNum2 = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();

        // Perform binary addition directly at the bitset level
        bitset<8> result = reg[regNum1].get() ^ reg[regNum2].get();  // XOR for bitwise sum
        bitset<8> carry = reg[regNum1].get() & reg[regNum2].get();  // AND for carry bits
        while (carry.any()) {  // Propagate carry if any
            bitset<8> newCarry = carry << 1;
            carry = result & newCarry;
            result ^= newCarry;
        }

        reg[regNum1].load(result);
        break;
    }
    case 0x03: {  // SUB (Binary Subtraction)
        uint8_t regNum1 = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        uint8_t regNum2 = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();

        // Perform binary subtraction directly at the bitset level (using 2's complement)
        bitset<8> result = reg[regNum1].get() ^ (~reg[regNum2].get()) ^ bitset<8>(1);
        bitset<8> borrow = (~reg[regNum1].get()) & reg[regNum2].get();
        while (borrow.any()) {  // Propagate borrow if any
            bitset<8> newBorrow = borrow << 1;
            borrow = (~result) & newBorrow;
            result ^= newBorrow;
        }

        reg[regNum1].load(result);
        break;
    }
    case 0x0A: {  // WRITE
        uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        cout << "Output: " << reg[regNum].get().to_ulong() << endl;
        break;
    }
    case 0x09: {  // READ
        uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        int input;
        cout << "Input: ";
        cin >> input;
        reg[regNum].load(bitset<8>(input));
        break;
    }
    case 0x0B: {  // JMP
        uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        pc.load(bitset<8>(address));
        break;
    }
    case 0x0C: {  // CALL
        uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        callStack.push(pc.get().to_ulong());
        pc.load(bitset<8>(address));
        break;
    }
    case 0x0D: {  // RET
        if (!callStack.empty()) {
            pc.load(bitset<8>(callStack.top()));
            callStack.pop();
        } else {
            cout << "Error: Call stack underflow!" << endl;
        }
        break;
    }
    case 0x0E: {  // BEQ
        uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        if (reg[regNum].get().to_ulong() == 0) {
            pc.load(bitset<8>(address));
        }
        break;
    }
    case 0x0F: {  // BNE
        uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
        pc.increment();
        if (reg[regNum].get().to_ulong() != 0) {
            pc.load(bitset<8>(address));
        }
        break;
    }
    case 0x10: {  // INT
        cout << "Interrupt triggered!" << endl;
        break;
    }
    case 0x00: {  // NOP
        break;
    }
    default:
        cout << "Error: Unknown opcode " << (int)opcode << endl;
}

        }
        auto stop = high_resolution_clock::now(); // Stop timer
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Execution time: " << duration.count() << " microseconds" << endl;
    }

    void displayState() {
        cout << "Registers: \n";
        for (int i = 0; i < 4; ++i) {
            cout << "R" << i << ": " << reg[i].get() << endl;
        }
       // cout << "Program Counter: " << pc.get() << endl;
        cout << "Memory State: \n";
        memory.displayMemory();
    }
};

int main() {
    // Example assembly program
    vector<string> assembly = {
        "LOAD 0 15",   // Load 15 into R0
        "LOAD 1 8",    // Load 8 into R1
        "ADD 0 1",     // Add R1 to R0
        "READ 1",      // Input to R1
        "WRITE 0",     // Output R0
        "STORE 1 100", // Store R1 at memory address 100
        "NOP"          // No operation
    };

    // Assemble the program into binary
    vector<bitset<8>> machineCode = assembleBinary(assembly);

    cout << "Machine code (Binary): \n";
    for (const auto& byte : machineCode) {
        cout << byte << " ";
    }
    cout << endl;

    // Run the program
    CPU cpu;
    cpu.execute(machineCode);

    // Display the final state
    cpu.displayState();

    return 0;
}
