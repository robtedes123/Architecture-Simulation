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
Cache::read(uint32_t address, Line* line) {
    const uint32_t idx = INDEX(address);
    const uint32_t tag = TAG(address);

    if (lines[idx].tag == tag && lines[idx].valid) {
        *line = line[idx];
        return true;
    } else {
        return false;
    }
}

void
Cache::write(uint32_t address, Line line) {
    const uint32_t idx = INDEX(address);

    lines[idx] = line;
}

void
Memory::loadProgram(const vector<uint32_t>& program) {
    // load program starting at address 0
    for (int i = 0; i < program.size(); i++) {
        ram[i] = program[i];
        printf("%x\n", program[i]);
    }

    // put a marker at end of program
    ram[program.size()] = ~0;
}

uint32_t
Memory::read(uint32_t address, uint32_t* cycles) {
    address /= sizeof(uint32_t);

    *cycles += RAM_DELAY;
    return ram[address];
}

void
Memory::write(uint32_t address, uint32_t data, uint32_t* cycles) {
    address /= sizeof(uint32_t);

    // writethrough - no allocate
    for (auto& cache : caches) {
        *cycles += cache.DELAY;
        cache.invalidate(address);
    }

    *cycles += RAM_DELAY;
    ram[address] = data;
}

//Cache::Line
//Memory::readLine(uint32_t address, uint32_t* cycles) {
//    Cache::Line line;
//    bool hit;
//
//    int i;
//    for (i = 0; i < caches.size(); i++) {
//        auto& cache = caches[i];
//
//        *cycles += cache.DELAY;
//        hit = cache.read(address, &line);
//
//        if (hit) {
//            printf("HIT");
//            break;
//        }
//    }
//
//    if (!hit) {
//        *cycles += RAM_DELAY;
//        byte = ram[address];
//    }
//
//    for (i--; i >= 0; i--) {
//        auto& cache = caches[i];
//
//        *cycles += cache.DELAY;
//        cache.write(address, line);
//    }
//
//    return byte;
//}
