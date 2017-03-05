// Class for running simulation

#include "Memory.h"

int main()
{
    Memory memory {
        { .NAME = "L1" , .SIZE =  32 },
        { .NAME = "L2" , .SIZE =  64 },
        { .NAME = "L3" , .SIZE = 128 },
        { .NAME = "RAM", .SIZE = 256 },
    };

    memory.printHierarchy();
}
