#include "Mem.h"

#include <cstdio>

void
Cache::invalidate(uint32_t address) {
    const int idx = INDEX(address);
    const int tag = TAG(address);

    if (lines[idx].tag == tag) {
        lines[idx].valid = false;
    }
}

bool
Cache::read(uint32_t address, uint8_t* val) {
    const uint32_t idx = INDEX(address);
    const uint32_t tag = TAG(address);

    if (lines[idx].tag == tag && lines[idx].valid) {
        const uint32_t offset = OFFSET(address);
        const uint32_t byte   = BYTE(address);

        *val = (lines[idx].line[offset] >> (8*offset)) & 0xFF;

        return true;
    } else {
        return false;
    }
}

void
Memory::loadProgram(const vector<uint32_t>& program) {
    // load program starting at address 0
    for (int i = 0; i < program.size(); i++) {
        uint32_t word = program[i];
        ram[i] = word;
        //ram[4*i + 0] = (word >>  0) & 0xFF;
        //ram[4*i + 1] = (word >>  8) & 0xFF;
        //ram[4*i + 2] = (word >> 16) & 0xFF;
        //ram[4*i + 3] = (word >> 24) & 0xFF;
    }

    ram[i] = ~0;

    //ram[4*program.size() + 0] = ~0;
    //ram[4*program.size() + 1] = ~0;
    //ram[4*program.size() + 2] = ~0;
    //ram[4*program.size() + 3] = ~0;
}

uint8_t
Memory::read(uint32_t address, uint32_t* cycles) {
    Line line;
    bool hit;

    int i;
    for (i = 0; i < caches.size(); i++) {
        auto& cache = caches[i];

        *cycles += cache.DELAY;
        hit = cache.read(address, &line);

        if (hit) {
            printf("HIT");
            break;
        }
    }

    if (!hit) {
        *cycles += RAM_DELAY;
        byte = ram[address];
    }

    for (i--; i >= 0; i--) {
        auto& cache = caches[i];

        *cycles += cache.DELAY;
        cache.write(address, line);
    }

    return byte;
}

void
Memory::write(uint32_t address, uint8_t byte, uint32_t* cycles) {
    // writethrough - no allocate
    for (auto& cache : caches) {
        *cycles += cache.DELAY;
        cache.invalidate(address);
    }

    *cycles += RAM_DELAY;
    ram[address] = byte;
}

