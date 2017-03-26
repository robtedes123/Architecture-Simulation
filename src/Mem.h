#pragma once

#include "config.h"

#include <array>
#include <vector>

#include <cstdint>

using namespace std;

class Memory
{
    public:
        void loadProgram(const vector<uint32_t>& program);

        uint8_t read(uint32_t address, int* delay);
        void write(uint32_t address, uint8_t byte, int* delay);

    private:
        array<uint8_t, RAM_SIZE> mem;
};

