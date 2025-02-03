#ifndef ALU_H
#define ALU_H

#include <cstdint>

class ALU {
public:
    uint8_t add(uint8_t a, uint8_t b);
    uint8_t sub(uint8_t a, uint8_t b);
    uint8_t and_op(uint8_t a, uint8_t b);
    uint8_t or_op(uint8_t a, uint8_t b);
    uint8_t xor_op(uint8_t a, uint8_t b);
    uint8_t nop();
};

#endif // ALU_H
