#include "Memory.h"

Memory::Memory(initializer_list<Config> args) :
    Memory(std::queue<Config>(args), nullptr) {}

Memory::Memory(std::queue<Config> configs, const Memory* prev) :
    name(configs.front().NAME),
    size(configs.front().SIZE),
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

void Memory::printHierarchy()
{
    for (const Memory* level = this; level != nullptr; level = level->next) {
        cout << level->name << " : " << level->size << " bytes" << endl;
    }
}
