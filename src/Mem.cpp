#include "Mem.h"

void
Memory::loadProgram(const vector<uint32_t>& program) {
    // load program starting at address 0
    int i;
    for (i = 0; i < program.size(); i++) {
        uint32_t word = program[i];
        mem[4*i + 0] = (word >>  0) & 0xFF;
        mem[4*i + 1] = (word >>  8) & 0xFF;
        mem[4*i + 2] = (word >> 16) & 0xFF;
        mem[4*i + 3] = (word >> 24) & 0xFF;
    }

    mem[4*i + 0] = ~0;
    mem[4*i + 1] = ~0;
    mem[4*i + 2] = ~0;
    mem[4*i + 3] = ~0;
}

uint8_t
Memory::read(uint32_t address, int* delay) {
    *delay += RAM_DELAY;
    return mem[address];
}

void
Memory::write(uint32_t address, uint8_t byte, int* delay) {
    *delay += RAM_DELAY;
    mem[address] = byte;
}

