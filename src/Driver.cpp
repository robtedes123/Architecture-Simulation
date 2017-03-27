// Class for running simulation

#include "Cpu.h"

int
main(int argc, char** argv) {
    // TODO: read program from file

    // HACK
    const vector<uint32_t> program = {
        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00000 << 20 | 10),
        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00001 << 20 | 100),
        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00010 << 20 | 0),
        (uint32_t)(0b00 << 30 | 0b00101 << 25 | 0b00000 << 20 | 0b00001 << 15),
        //(uint32_t)(0b10 << 30 | 0b00    << 28 | 0b111111111111111111111100),
        (uint32_t)(0b10 << 30 | 0b00    << 28 | 4),
        (uint32_t)(0b10 << 30 | 0b00    << 28 | 0),
    };

    CPU cpu(program);
    int num_cycles = cpu.exec();

    printf("num cycles = %d\n", num_cycles);
}
