#include "Assembler.h"
#include <sstream>
#include <iostream>

Assembler::Assembler() {
    // Initialize opcode map
    opcodeMap = {
        {"LOAD", 0x01}, {"ADD", 0x02}, {"SUB", 0x03}, {"STORE", 0x04}, {"NOP", 0x05},
        {"AND", 0x06}, {"OR", 0x07}, {"XOR", 0x08}, {"READ", 0x09}, {"WRITE", 0x0A},
        {"JMP", 0x0B}, {"CALL", 0x0C}, {"RET", 0x0D}, {"BEQ", 0x0E}, {"BNE", 0x0F}, {"INT", 0x10}
    };

    // Initialize instruction operand map
    instructionOperands = {
        {"LOAD", 2}, {"ADD", 2}, {"SUB", 2}, {"STORE", 2}, {"NOP", 0},
        {"AND", 2}, {"OR", 2}, {"XOR", 2}, {"READ", 1}, {"WRITE", 1},
        {"JMP", 1}, {"CALL", 1}, {"RET", 0}, {"BEQ", 1}, {"BNE", 1}, {"INT", 0}
    };
}

std::vector<std::string> Assembler::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string trimmedLine = line.substr(0, line.find('#'));  // Remove comments
    std::istringstream stream(trimmedLine);
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<uint8_t> Assembler::assemble(const std::vector<std::string>& assemblyCode) {
    std::vector<uint8_t> machineCode;

    for (const auto& line : assemblyCode) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        // Validate instruction
        if (opcodeMap.find(tokens[0]) == opcodeMap.end()) {
            std::cerr << "Error: Unknown instruction: " << tokens[0] << std::endl;
            continue;
        }

        // Add opcode to machine code
        machineCode.push_back(opcodeMap[tokens[0]]);

        // Validate operand count
        int expectedOperands = instructionOperands[tokens[0]];
        if (tokens.size() - 1 != expectedOperands) {
            std::cerr << "Error: Incorrect number of operands for " << tokens[0] << std::endl;
            continue;
        }

        // Convert operands to numeric values and add to machine code
        try {
            for (size_t i = 1; i < tokens.size(); ++i) {
                machineCode.push_back(static_cast<uint8_t>(std::stoi(tokens[i])));
            }
        } catch (...) {
            std::cerr << "Error: Invalid operand in instruction: " << tokens[0] << std::endl;
        }
    }
    return machineCode;
}
