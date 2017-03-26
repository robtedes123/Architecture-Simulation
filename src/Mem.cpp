#include "Mem.h"

void
Memory::loadProgram(uint32_t address, const vector<uint32_t>& program) {
    for (int i = 0; i < program.size(); i++) {
        uint32_t word = program[i];
        mem[address+(4*i)+0] = (word >>  0) & 0xFF;
        mem[address+(4*i)+1] = (word >>  8) & 0xFF;
        mem[address+(4*i)+2] = (word >> 16) & 0xFF;
        mem[address+(4*i)+3] = (word >> 24) & 0xFF;
    }
}

uint8_t
Memory::read(uint32_t address, int* delay) {
    *delay += this->delay;
    return mem[address];
}

void
Memory::write(uint32_t address, uint8_t byte, int* delay) {
    *delay += this->delay;
    mem[address] = byte;
}

