#ifndef _MEMORY_H
#define _MEMORY_H

// Class representing memory operations for Architecture Simulation

#include <cstdint>
#include <string>
#include <queue>
#include <initializer_list>

using namespace std;

struct Config {
    const string name;
    const int    size;
    const int    delay;
};

class Memory
{
    public:
        Memory(initializer_list<Config>);
        ~Memory();

        void printHierarchy();
    private:
        Memory(std::queue<Config>, const Memory*);

        const string name;

        const int size;
        uint8_t*  data;

        const int delay;

        const Memory* prev;
        const Memory* next;
};

#endif // _MEMORY_H
