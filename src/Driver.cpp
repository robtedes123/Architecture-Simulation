// Class for running simulation

#include "Cpu.h"

int
main(int argc, char** argv) {
    const vector<uint32_t> program = {65536, 100, 100};

    CPU cpu(program);
    cpu.execute();
}
