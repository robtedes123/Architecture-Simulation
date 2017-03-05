#ifndef _MEMORY_H
#define _MEMORY_H

// Class representing memory operations for Architecture Simulation

#include <cstdint>
#include <queue>
#include <initializer_list>

using namespace std;

struct Config {
    // Name of the memory layer (used for debugging purposes)
    // NOTE: You have to provide a static string
    const char* name;

    // Size of the memory layer in bytes
    const int size;

    // Number cycles required to process a request
    const int delay;
};

class Memory
{
    public:
        Memory(initializer_list<Config>);
        ~Memory();

        // reads a byte from adddress in memory
        // returns the number of cycles required to perform operation
        int read(int address, uint8_t* out_byte) const;

        // writes a byte to address in memory
        // returns the number of cycles required to perform operation
        int write(int address, uint8_t byte);

        // prints out the memory structure (used for debugging purposes)
        void printHierarchy();
    private:
        Memory(std::queue<Config>, const Memory*);

        // Name of the memory layer (used for debugging purposes)
        const char* name;

        // Size of the memory layer in bytes
        const int size;

        // Number cycles required to process a request
        const int delay;

        // Internal representation of memory
        uint8_t* data;

        // Previous level of memory
        const Memory* prev;

        // Next level of memory
        const Memory* next;
};

#endif // _MEMORY_H
