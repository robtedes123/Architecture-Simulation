// Class for running simulation

#include "Cpu.h"

int
main(int argc, char** argv) {
    // TODO: read program from file

    // HACK
    const vector<uint32_t> program = {
        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00000 << 20 | 10),
        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00000 << 20 | 100),
        (uint32_t)(0b00 << 30 | 0b00000 << 25 | 0b00010 << 20 | 0b00000 << 15 | 0b00001 << 10),
    };

    CPU cpu(program);
    int num_cycles = cpu.exec();

    printf("num cycles = %d\n", num_cycles);
}
