#include "CPU.h"
#include <iostream>
#include <bitset>
using namespace std;

void CPU::fetchInstruction(const std::vector<uint8_t>& machineCode) {
    if (pc.get() < machineCode.size()) {
        ir.load(machineCode[pc.get()]);
    } else {
        std::cout << "Error: Program counter out of bounds!" << std::endl;
    }
}

void CPU::execute(const std::vector<uint8_t>& machineCode) {
    std::vector<uint8_t> interruptStack;
    while (pc.get() < machineCode.size()) {
        fetchInstruction(machineCode);
        uint8_t opcode = ir.get();
        pc.increment();  // Increment program counter after each instruction

        switch (opcode) {
            // Case logic for different instructions...
             case 0x01: { // LOAD
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    registers.write(regIndex, operand);
                    pc.increment();
                    pc.increment();
                    break;
                }
                case 0x02: { // ADD
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    registers.write(regIndex, alu.add(registers.read(regIndex), operand));
                    pc.increment();
                    pc.increment();
                    break;
                }
                case 0x03: { // SUB
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    registers.write(regIndex, alu.sub(registers.read(regIndex), operand));
                    pc.increment();
                    pc.increment();
                    break;
                }
                case 0x04: { // STORE
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t memoryAddress = machineCode[pc.get() + 1];
                    memory.write(memoryAddress, registers.read(regIndex));
                    pc.increment();
                    pc.increment();
                    break;
                }
                case 0x05: { // NOP
                    break;
                }
                case 0x06: { // AND
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    registers.write(regIndex, alu.and_op(registers.read(regIndex), operand));
                    pc.increment();
                    pc.increment();
                    break;
                }
                case 0x07: { // OR
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    registers.write(regIndex, alu.or_op(registers.read(regIndex), operand));
                    pc.increment();
                    pc.increment();
                    break;
                }
                case 0x08: { // XOR
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t operand = machineCode[pc.get() + 1];
                    uint8_t result = alu.xor_op(registers.read(regIndex), operand);
                    registers.write(regIndex, result);
                    pc.increment();
                    pc.increment();
                    break;
                }
                case 0x09: { // READ (keyboard)
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t input = inputDevice.read();
                    registers.write(regIndex, input);
                    pc.increment();
                    break;
                }
                case 0x0A: { // WRITE (display)
                    uint8_t regIndex = machineCode[pc.get()];
                    uint8_t value = registers.read(regIndex);
                    outputDevice.write(value);
                    pc.increment();
                    break;
                }
                case 0x0B: { // JMP
                    uint8_t address = machineCode[pc.get()];
                    pc.set(address);
                    break;
                }
                case 0x0C: { // CALL
                    uint8_t address = machineCode[pc.get()];
                    callStack.push(registers.read(0)); // Save return address
                    pc.set(address);
                    break;
                }
                case 0x0D: { // RET
                    if (!callStack.empty()) {
                        uint8_t returnAddr = callStack.top();
                        callStack.pop();
                        pc.set(returnAddr);
                    } else {
                        cout << "Error: Call stack underflow!" << endl;
                    }
                    break;
                }
                case 0x0E: { // BEQ (Branch if equal)
                    uint8_t address = machineCode[pc.get()];
                    if (registers.read(0) == registers.read(1)) {
                        pc.set(address);
                    }
                    pc.increment();
                    break;
                }
                case 0x0F: { // BNE (Branch if not equal)
                    uint8_t address = machineCode[pc.get()];
                    if (registers.read(0) != registers.read(1)) {
                        pc.set(address);
                    }
                    pc.increment();
                    break;
                }
                case 0x10: { // INT (Interrupt)
                    interruptStack.push_back(pc.get());
                    pc.increment();
                    break;
                }
                default:
                    cout << "Error: Unknown opcode " << (int)opcode << endl;
                    break;
        }
    }
}

void CPU::displayState() {
    std::cout << "Registers:" << std::endl;
    registers.display();
    std::cout << "Memory:" << std::endl;
    memory.displayMemory();
}
