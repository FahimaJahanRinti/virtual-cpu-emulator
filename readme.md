# Virtual CPU Emulator

This project emulates a simplified CPU in C++. Over 10 weeks, we’ll build essential CPU components, implement a custom instruction set, and simulate core CPU functionality. The emulator will support basic arithmetic, logic, memory operations, and control flow to understand CPU operations.

## Weekly Outline

### Week 1: Project Planning & Setup

- Define project goals and CPU features.
- Set up development environment and version control.
- Select tools and libraries for C++ development.

### Week 2: Instruction Set Architecture (ISA)

- Design the instruction set with opcodes for `ADD`, `SUB`, `LOAD`, `STORE`, etc.
- Outline instruction formats and create a basic assembler to convert assembly code to binary.

### Week 3: Core CPU Components

- Implement essential components:
  - **ALU (Arithmetic Logic Unit)** for arithmetic/logic operations.
  - **Registers** for temporary data storage.
  - **Program Counter (PC)** and **Instruction Register (IR)**.

### Week 4: Instruction Execution Cycle

- Develop the fetch-decode-execute cycle.
- Implement functionality to fetch instructions, decode opcodes, and execute operations with the ALU.

### Week 5: Memory Management

- Set up simulated memory for storing data and instructions.
- Implement memory read/write operations and address mapping.

### Week 6: I/O Operations

- Simulate basic I/O operations (keyboard, display).
- Integrate I/O handling instructions into the CPU.

### Week 7: Control Flow and Branching

- Implement branching instructions (`JUMP`, `JZ`, `JNZ`) and function calls.
- Add support for control flow with stack-based subroutine handling.

### Week 8: Performance Optimization

- Profile code to identify bottlenecks.
- Optimize critical operations and enhance assembler instruction encoding.

### Week 9: Testing and Debugging

- Conduct unit tests on each CPU component.
- Run integration tests with complete programs, debug, and validate.

### Week 10: Documentation & Presentation

- Prepare project documentation and final report.
- Create a demo session to showcase CPU operations and emulator features.


