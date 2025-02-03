#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

class Assembler {
private:
    std::unordered_map<std::string, uint8_t> opcodeMap;  // Maps mnemonics to opcodes
    std::unordered_map<std::string, int> instructionOperands;  // Operand count map

public:
    Assembler();  // Constructor to initialize the opcode map and operand counts
    std::vector<std::string> tokenize(const std::string& line);  // Tokenize a single line of assembly
    std::vector<uint8_t> assemble(const std::vector<std::string>& assemblyCode);  // Assemble multiple lines
};

#endif // ASSEMBLER_H
