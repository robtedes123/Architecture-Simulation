#pragma once

#include <vector>

#include <cstdint>

using namespace std;

class Memory
{
    public:
        Memory(int size, int delay) :
            mem(vector<uint8_t>(size)),
            delay(delay)
        {
        }

        void loadProgram(uint32_t address, const vector<uint32_t>& program);

        uint8_t read(uint32_t address, int* delay);
        void write(uint32_t address, uint8_t byte, int* delay);

    private:
        vector<uint8_t> mem;
        const int delay;
};

