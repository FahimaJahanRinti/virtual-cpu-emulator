#Virtual CPU Emulator:
This project implements a simple 8-bit Virtual CPU Emulator in C++. The emulator supports basic arithmetic operations, memory handling, branching, and I/O. It includes an assembler to convert assembly instructions into binary machine code.

#Features:
1.Assembler that converts assembly code into machine code.  
2.Memory management with read/write operations.  
3.CPU registers and program counter.  
4.Execution of arithmetic and logical instructions.  
5.Handling of branching, function calls, and stack operations
Input/Output operations.  


#Example Assembly Code Execution:
The main function demonstrates assembling and running an assembly program.


```
int main() {
vector<string> assembly = {
"LOAD 0 15",
"LOAD 1 8",
"ADD 0 1",
"WRITE 0"
};
vector<bitset<8>> machineCode = assembleBinary(assembly);
CPU cpu;
cpu.execute(machineCode);
return 0;
}
```

#Purpose:
1.Loads values into registers.
2.Performs an addition operation.
3.Outputs the result using the WRITE instruction.



#Tokenization:
This function tokenizes each line of assembly code into individual components.


```
vector<string> tokenize(const string& line) {
vector<string> tokens;
stringstream ss(line);
string token;
while (ss >> token) {
tokens.push_back(token);
}
return tokens;
}

```

#Purpose:
1.Splits an assembly instruction into separate words.
2.Helps in parsing and identifying the instruction and its operands.




#Opcode and Operand Mapping:
Defines the instruction set architecture (ISA) by mapping mnemonics to opcodes and tracking operand counts.


```

unordered_map<string, uint8_t> opcodeMap = {
{"LOAD", 0x01}, {"ADD", 0x02}, {"SUB", 0x03},
{"STORE", 0x04}, {"NOP", 0x00}, {"AND", 0x06},
{"OR", 0x07}, {"XOR", 0x08}, {"READ", 0x09},
{"WRITE", 0x0A}, {"JMP", 0x0B}, {"CALL", 0x0C},
{"RET", 0x0D}, {"BEQ", 0x0E}, {"BNE", 0x0F},
{"INT", 0x10}
};


```
#Purpose:
1.Maps instruction names to their corresponding opcode values.
2.Used for assembling code into machine code.



#Assembler: Converts Assembly to Machine Code
The assembleBinary function translates assembly instructions into binary machine code.

```

vector<bitset<8>> assembleBinary(const vector<string>& assembly) {
vector<bitset<8>> machineCodeBinary;
for (const auto& line : assembly) {
auto tokens = tokenize(line);
if (tokens.empty()) continue;

if (opcodeMap.find(tokens[0]) == opcodeMap.end()) {
cout << "Error: Unknown instruction: " << tokens[0] << endl;
continue;
}

machineCodeBinary.push_back(bitset<8>(opcodeMap[tokens[0]]));
for (size_t i = 1; i < tokens.size(); ++i) {
machineCodeBinary.push_back(bitset<8>(stoi(tokens[i])));
}
}
return machineCodeBinary;
}


```


#Purpose:
1.Converts each instruction into its binary representation.
2.Ensures proper operand count and handles errors.



#Memory Management:
Defines a Memory class to simulate RAM with read/write operations.

```


class Memory {
private:
bitset<8> mem[256];
public:
void write(uint8_t address, const bitset<8>& value) {
mem[address] = value;
}
bitset<8> read(uint8_t address) {
return mem[address];
}
};

```


#Purpose:
1.Stores program instructions and data.
2.Provides a simple interface for reading and writing memory values.




#Register Management:
Defines a Register class to store 8-bit values.

```


class Register {
private:
bitset<8> value;
public:
void load(const bitset<8>& val) { value = val; }
bitset<8> get() const { return value; }
};

```



#Purpose:

1.Holds values for CPU operations.
2.Provides methods for loading and retrieving values.




#CPU Execution:
Handles the instruction execution cycle (fetch, decode, execute).



```

class CPU {
private:
Register pc; // Program counter (binary)
Register ir; // Instruction register (binary)
Register reg[4]; // Four general-purpose registers (binary)
Memory memory;
stack<uint8_t> callStack; // Stack for CALL and RET instructions

void fetchInstruction(const vector<bitset<8>>& machineCode) {
if (pc.get().to_ulong() < machineCode.size()) {
ir.load(machineCode[pc.get().to_ulong()]); // Fetch the instruction in binary
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
case 0x01: { // LOAD
uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
uint8_t value = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
reg[regNum].load(bitset<8>(value));
break;
}
case 0x04: { // STORE
uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
memory.write(address, reg[regNum].get());
break;
}
case 0x02: { // ADD (Binary Addition)
uint8_t regNum1 = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
uint8_t regNum2 = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();

// Perform binary addition directly at the bitset level
bitset<8> result = reg[regNum1].get() ^ reg[regNum2].get(); // XOR for bitwise sum
bitset<8> carry = reg[regNum1].get() & reg[regNum2].get(); // AND for carry bits
while (carry.any()) { // Propagate carry if any
bitset<8> newCarry = carry << 1;
carry = result & newCarry;
result ^= newCarry;
}

reg[regNum1].load(result);
break;
}
case 0x03: { // SUB (Binary Subtraction)
uint8_t regNum1 = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
uint8_t regNum2 = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();

// Perform binary subtraction directly at the bitset level (using 2's complement)
bitset<8> result = reg[regNum1].get() ^ (~reg[regNum2].get()) ^ bitset<8>(1);
bitset<8> borrow = (~reg[regNum1].get()) & reg[regNum2].get();
while (borrow.any()) { // Propagate borrow if any
bitset<8> newBorrow = borrow << 1;
borrow = (~result) & newBorrow;
result ^= newBorrow;
}

reg[regNum1].load(result);
break;
}
case 0x0A: { // WRITE
uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
cout << "Output: " << reg[regNum].get().to_ulong() << endl;
break;
}
case 0x09: { // READ
uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
int input;
cout << "Input: ";
cin >> input;
reg[regNum].load(bitset<8>(input));
break;
}
case 0x0B: { // JMP
uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
pc.load(bitset<8>(address));
break;
}
case 0x0C: { // CALL
uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
callStack.push(pc.get().to_ulong());
pc.load(bitset<8>(address));
break;
}
case 0x0D: { // RET
if (!callStack.empty()) {
pc.load(bitset<8>(callStack.top()));
callStack.pop();
} else {
cout << "Error: Call stack underflow!" << endl;
}
break;
}
case 0x0E: { // BEQ
uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
if (reg[regNum].get().to_ulong() == 0) {
pc.load(bitset<8>(address));
}
break;
}
case 0x0F: { // BNE
uint8_t regNum = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
uint8_t address = machineCode[pc.get().to_ulong()].to_ulong();
pc.increment();
if (reg[regNum].get().to_ulong() != 0) {
pc.load(bitset<8>(address));
}
break;
}
case 0x10: { // INT
cout << "Interrupt triggered!" << endl;
break;
}
case 0x00: { // NOP
break;
}
default:
cout << "Error: Unknown opcode " << (int)opcode << endl;
}

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


```



#Purpose:
1.Simulates the CPU’s fetch-decode-execute cycle.
2.Executes different instructions like arithmetic, branching, and I/O.
3.Measures execution time and prints it.
4.Displays register values and memory contents.


















