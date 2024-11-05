#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

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
    {"LOAD", 1},
    {"ADD", 1},
    {"SUB", 1},
    {"STORE", 1},
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

// In the assemble function, consider adding more checks and handling more cases
vector<uint8_t> assemble(const vector<string>& assembly) {
    vector<uint8_t> machineCode;
    for (const auto& line : assembly) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue; // Skip empty lines

        // Check if instruction exists in the opcode map
        if (opcodeMap.find(tokens[0]) == opcodeMap.end()) {
            cout << "Error Unknown instruction: " << tokens[0] << endl;
            continue;  // Skip this line
        }

        // Add opcode to machine code
        machineCode.push_back(opcodeMap[tokens[0]]);

        // Check for operands based on instruction
        if (tokens.size() > 1) {
            try {
                // Check if the instruction requires an operand
                if (tokens[0] == "LOAD" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "STORE") {
                    machineCode.push_back(static_cast<uint8_t>(stoi(tokens[1])));
                } else {
                    cout << "Error Instruction " << tokens[0] << " should not have an operand." << endl;
                }
            } catch (const invalid_argument& e) {
                cout << "Error Invalid operand for instruction: " << tokens[0] << endl;
            } catch (const out_of_range& e) {
                cout << "Error Operand value out of range for instruction: " << tokens[0] << endl;
            }
        }
    }
    return machineCode;
}

// Main function to demonstrate the assembler
int main() {
    vector<string> assembly = {
        "LOAD 5          # Load 5 into register",
        "ADD 3           # Add 3 to the accumulator",
        "STORE 8         # Store result at memory address 8",
        "NOP             # No operation",
        "SUB 2           # Extra operand should trigger error",
        "UNKNOWN 4       # Unknown instruction"
    };

    vector<uint8_t> machineCode = assemble(assembly);

    // Display machine code
    cout << "Machine code: ";
    for (uint8_t byte : machineCode) {
        cout << hex << static_cast<int>(byte) << " ";
    }
    cout << endl;

    return 0;
}