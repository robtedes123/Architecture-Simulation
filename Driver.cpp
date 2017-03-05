// Class for running simulation

#include "Memory.h"

#include <QApplication>
#include <QWidget>

using namespace std;

int main(int argc, char** argv)
{
    Memory memory {
        { .name = "L1"  , .size = 0x0500, .delay = 1 },
        { .name = "RAM" , .size = 0x1000, .delay = 2 },
    };

    QApplication app(argc, argv);

    QWidget window;
    window.resize(250,150);
    window.setWindowTitle("Simple example");
    window.show();

    return app.exec();
}
