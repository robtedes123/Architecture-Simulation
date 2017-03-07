#include "Memory.h"

#include <cstdio>

Controller::Controller(const vector<Memory*> layers) :
    layers(layers)
{
}

int
Controller::read(const int address, uint8_t* byte) const
{
    int delay = 0;
    for (Memory* l : layers) {
        delay += l->DELAY;

        bool hit = l->read(address, byte);

        if (hit) return delay;
    }

    puts("READ ERROR");
    return -1;
}

int
Controller::write(const int address, uint8_t byte)
{
    Memory* level = layers.back(); // write directly to RAM now
    level->write(address, byte);
    return level->DELAY;
}

Cache::Cache(const int delay) : Memory(delay)
{
}

bool
Cache::read(const int address, uint8_t* byte) const
{
    //TODO
    return false;
}

void
Cache::write(const int address, const uint8_t byte)
{
    //TODO
}

RAM::RAM(const int size, const int delay) :
    Memory(delay),
    memory(vector<uint8_t>(size))
{
}

bool
RAM::read(const int address, uint8_t* byte) const
{
    *byte = memory[address];
    return true;
}

void
RAM::write(const int address, const uint8_t byte)
{
    memory[address] = byte;
}
