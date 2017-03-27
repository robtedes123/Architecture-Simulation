// Class for running simulation

#include "Cpu.h"

#include <fstream>

int
main(int argc, char** argv) {
    const vector<uint32_t> program = {
        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00000 << 20 | 10)
    };

    CPU cpu(program);
    int num_cycles = cpu.exec();

    printf("num cycles = %d\n", num_cycles);
}
