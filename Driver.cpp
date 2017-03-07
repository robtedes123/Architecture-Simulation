// Class for running simulation

#include "Memory.h"

#include <cstdint>
#include <cstdio>

using namespace std;

int main(int argc, char** argv)
{
    Controller controller({
        new Cache(10),
        new Cache(20),
        new RAM(0x2000, 100)
    });

    uint8_t byte;
    int delay;

    delay = controller.read(0x1000, &byte);
    printf("R @ 0x1000 : data = %3d : delay = %3d cycles\n", byte, delay);

    byte = 10;
    delay = controller.write(0x1000, byte);
    printf("W @ 0x1000 : data = %3d : delay = %3d cycles\n", byte, delay);

    delay = controller.read(0x1000, &byte);
    printf("R @ 0x1000 : data = %3d : delay = %3d cycles\n", byte, delay);
}
