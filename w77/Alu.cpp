#include "ALU.h"

uint8_t ALU::add(uint8_t a, uint8_t b) { return (a + b) & 0xFF; }
uint8_t ALU::sub(uint8_t a, uint8_t b) { return (a - b) & 0xFF; }
uint8_t ALU::and_op(uint8_t a, uint8_t b) { return a & b; }
uint8_t ALU::or_op(uint8_t a, uint8_t b) { return a | b; }
uint8_t ALU::xor_op(uint8_t a, uint8_t b) { return a ^ b; }
uint8_t ALU::nop() { return 0; }
