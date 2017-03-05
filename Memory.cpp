#include "Memory.h"

#include <cstdio>

Memory::Memory(initializer_list<Config> args) :
    Memory(std::queue<Config>(args), nullptr) {}

Memory::Memory(std::queue<Config> configs, const Memory* prev) :
    name(configs.front().name),
    size(configs.front().size),
    delay(configs.front().delay),
    prev(prev)
{
    data = new uint8_t[size];

    configs.pop();
    if (configs.empty()) {
        next = nullptr;
    } else {
        next = new Memory(configs, this);
    }
}

Memory::~Memory()
{
    delete[] data;

    if (next != nullptr) {
        delete next;
    }
}

int
Memory::read(const int address, uint8_t* out_byte) const
{
    // TODO : look at next/prev level
    *out_byte = data[address];
    return delay;
}

int
Memory::write(const int address, uint8_t byte)
{
    // TODO : look at next/prev level
    data[address] = byte;
    return delay;
}

void
Memory::printHierarchy()
{
    puts("Memory Hierarchy:");
    for (const Memory* lvl = this; lvl != nullptr; lvl = lvl->next) {
        printf("%5s : %5d bytes : %3d cycle delay\n", lvl->name, lvl->size, lvl->delay);
    }
}
