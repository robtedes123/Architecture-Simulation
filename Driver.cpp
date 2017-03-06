// Class for running simulation

#include "Memory.h"

#include <cstdio>

using namespace std;

int main(int argc, char** argv)
{
    Memory* memory = new Memory({
        { .name = "L1"  , .size = 0x0500, .delay = 2 },
        { .name = "L2"  , .size = 0x1000, .delay = 20 },
        { .name = "L3"  , .size = 0x2000, .delay = 50 },
        { .name = "RAM" , .size = 0x4000, .delay = 100 },
    });

    memory->printHierarchy();

    //new line
    puts("");

    uint8_t byte = 0;
    int delay = 0;

    delay = memory->read(0x1000, &byte);
    printf("R @ 0x1000 : data = %3d : delay = %3d cycles\n", byte, delay);

    byte = 10;
    delay = memory->write(0x1000, byte);
    printf("W @ 0x1000 : data = %3d : delay = %3d cycles\n", byte, delay);

    delay = memory->read(0x1000, &byte);
    printf("R @ 0x1000 : data = %3d : delay = %3d cycles\n", byte, delay);

    //QApplication app(argc, argv);

    //QWidget window;
    //window.resize(500,400);
    //window.setWindowTitle("Architecture Simulation");
    //window.show();

    //return app.exec();
}
